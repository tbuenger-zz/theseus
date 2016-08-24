#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <unordered_map>
#include <type_traits>
#include <functional>
#include <iostream>
#include <cassert>



typedef std::string Symbol;

class Identifier
{
public:
    Identifier(Symbol symbol, bool isVariable) :
        symbol(symbol),
        isVariable(isVariable)
    { }

    Symbol symbol;
    bool isVariable;
};

inline std::ostream& operator<< (std::ostream& stream, const Identifier& identifier)
{
    return stream << identifier.symbol;
}


template <typename T>
class Singleton
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }

};

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class IdentifierFactory : public Singleton<IdentifierFactory>
{
private:
    std::unordered_map<Symbol, std::unique_ptr<Identifier>> identifiers;
    size_t counter=0;

    Identifier* getOrAdd(Symbol symbol, bool isVariable)
    {
        auto result = identifiers.emplace(symbol, nullptr);
        if (result.second)
        {
            result.first->second = make_unique<Identifier>(symbol, isVariable);
        }
        return result.first->second.get();
    }

    Symbol unusedSymbol()
    {
        std::string base = "_";
        Symbol symbol;
        do
        {
            symbol = base + std::to_string(counter++);
        } while(identifiers.find(symbol) != identifiers.end());
        return symbol;
    }

    Identifier* invent()
    {
        return getOrAdd(unusedSymbol(), false);
    }

public:

    Identifier* variable(Symbol symbol)
    {
        return getOrAdd(symbol, true);
    }

    Identifier* function(Symbol symbol)
    {
        return getOrAdd(symbol, false);
    };
};


template< class T > struct like_pointer                     : std::is_pointer<T> {};
template< class T > struct like_pointer<std::shared_ptr<T>> : std::true_type {};
template< class T > struct like_pointer<std::unique_ptr<T>> : std::true_type {};

template <typename T, typename Enable = void>
struct Printer
{
    static std::ostream& genericPrinter(std::ostream& stream, const T& t)
    {
        return stream << t;
    }  
};

template <typename T>
struct Printer<T, typename std::enable_if<like_pointer<T>::value>::type>
{
    static std::ostream& genericPrinter(std::ostream& stream, const T& t)
    {
        return stream << *t;
    }  
};

template <typename T>
struct Joiner
{
    const std::vector<T>& elements;
    const std::string& divider;
    const std::function<std::ostream&(std::ostream&, const T&)> printer;
};

template <typename T>
Joiner<T> join(const std::vector<T>& elements, const std::string& divider, std::ostream&(*printer)(std::ostream&, const T&))
{
    return {elements, divider, printer};
}

template <typename T>
Joiner<T> join(const std::vector<T>& elements, const std::string& divider)
{
    return {elements, divider, Printer<T>::genericPrinter};
}

template <typename T>
inline std::ostream& operator<< (std::ostream& stream, const Joiner<T>& j)
{
    int index = 0;
    for (auto& e : j.elements)
    {
        if (index++ > 0)
            stream << j.divider;
        j.printer(stream, e);
    }
    return stream;
}

class Term
{
public:
    Identifier* identifier;
    std::vector<Term*> children;

    Term(Identifier* identifier) :
        identifier(identifier),
        children()
    { }

    std::string toString() const
    {
        return (std::ostringstream() << *this).str();
    }
};

inline std::ostream& operator<< (std::ostream& stream, const Term& term)
{
    stream << *term.identifier;
    if (!term.children.empty())
    {
        stream << "(" << join(term.children, ",") << ")";
    }
    return stream;
}

enum Polarity
{
    Positive,
    Negative
};

class Literal
{
public:
    Polarity polarity;
    Term* term;

    Literal(Term* term) :
        polarity(Polarity::Positive),
        term(term)
    {}
};

inline std::ostream& operator<< (std::ostream& stream, const Literal& literal)
{
    if (literal.polarity == Polarity::Negative)
        stream << "~";
    stream << *literal.term;
    return stream;
}

class TermHasher
{   
public:

    // hash
    size_t operator()(const std::unique_ptr<Term>& t) const
    {
        size_t hash = 0;
        hash_combine(hash, reinterpret_cast<size_t>(&t));
        for (Term* child : t->children)
        {
            hash_combine(hash, reinterpret_cast<size_t>(child));
        }
        return hash;
    }

    // equal to
    bool operator()(const std::unique_ptr<Term>& lhs, const std::unique_ptr<Term>& rhs ) const
    {
        return lhs->identifier == rhs->identifier
            && lhs->children == rhs->children;
    }

private:
    inline void hash_combine(std::size_t& seed, std::size_t h) const
    {
        seed ^= h + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }
};

class TermDatabase : public Singleton<TermDatabase>
{
private:
    std::unordered_set<std::unique_ptr<Term>, TermHasher, TermHasher> known;
    std::vector<std::unique_ptr<Term>> heap;

    Term* singelize(std::unique_ptr<Term> term)
    {
        auto inserted = known.emplace(std::move(term));
        return inserted.first->get();
    }

public:

    Term* get(Identifier* identifier)
    {
        return singelize(make_unique<Term>(identifier));
    }

    Term* get(Identifier* identifier, std::vector<Term*> children)
    {
        auto result = make_unique<Term>(identifier);
        result->children = children;
        return singelize(std::move(result));
    }

    Term* substitute(Term* root, Identifier* needle, Term* replacement)
    {
        return substitute(root, get(needle), replacement);
    }

    Term* substitute(Term* root, Term* needle, Term* replacement)
    {
        if (root == needle)
            return replacement;
        
        for (size_t i = 0; i<root->children.size(); ++i)
        {
            Term* child = root->children[i];
            Term* new_child = substitute(child, needle, replacement);
            if (new_child != child)
            {
                auto result = make_unique<Term>(root->identifier);
                result->children = root->children;

                result->children[i] = new_child;
                for (size_t k = i+1; k<result->children.size(); ++k)
                {
                    result->children[k] = substitute(root->children[k], needle, replacement);
                }

                return singelize(std::move(result));
            }
        }
        return root;
    }
};

class Formula;
class FormulaVisitor;


class FormulaJunction;
class FormulaNegation;
class FormulaLiteral;
class FormulaQuantifier;
class FormulaImplication;



class FormulaVisitor
{
public:
    virtual void before(const FormulaJunction* op);
    virtual void before(const FormulaNegation* op);
    virtual void before(const FormulaLiteral* op);
    virtual void before(const FormulaQuantifier* op);
    virtual void before(const FormulaImplication* op);
    virtual void before(const Formula* op);

    virtual void between(const FormulaJunction* op);
    virtual void between(const FormulaNegation* op);
    virtual void between(const FormulaLiteral* op);
    virtual void between(const FormulaQuantifier* op);
    virtual void between(const FormulaImplication* op);
    virtual void between(const Formula* op);

    virtual std::shared_ptr<Formula> after(const FormulaJunction* op);
    virtual std::shared_ptr<Formula> after(const FormulaNegation* op);
    virtual std::shared_ptr<Formula> after(const FormulaLiteral* op);
    virtual std::shared_ptr<Formula> after(const FormulaQuantifier* op);
    virtual std::shared_ptr<Formula> after(const FormulaImplication* op);
    virtual std::shared_ptr<Formula> after(const Formula* op);
};

class Formula
{
public:
    virtual ~Formula() {}
    virtual std::shared_ptr<Formula> accept(FormulaVisitor&) = 0;
    virtual std::shared_ptr<Formula> accept(FormulaVisitor&) const = 0;

    friend std::ostream& operator<< (std::ostream& stream, const Formula& formula);

    inline std::string toString() const
    {
        return (std::ostringstream() << *this).str();
    }
};



template <typename T>
void acceptAndSwap(std::shared_ptr<T>& a, FormulaVisitor& v)
{
    auto result = a->accept(v);
    if (result)
        a = std::move(result);
}

template <typename T>
void acceptAndSwap(const std::shared_ptr<T>& a, FormulaVisitor& v)
{
    auto result = a->accept(v);
    if (result)
        throw std::invalid_argument("Visitor should not modify");
}

enum Operation
{
    Conjunction, // and
    Disjunction // or
};

class FormulaJunction : public Formula
{
public:
    Operation operation;
    std::vector<std::shared_ptr<Formula>> children;

    FormulaJunction(Operation operation, const std::vector<std::shared_ptr<Formula>> children) :
        operation(operation),
        children(std::move(children))
    {}

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) override
    {
        return accept(this, v);
    }

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) const override
    {
        return accept(this, v);
    }

private:

    template <typename T>
    static std::shared_ptr<Formula> accept(T* t, FormulaVisitor& v)
    {
        v.before(t);
        int index = 0;
        for (auto& child : t->children)
        {
            if (index++ > 0)
                v.between(t);
            acceptAndSwap(child, v);
        }
        return v.after(t);
    }
};

enum class ImplicationDirection
{
    Left,
    Right,
    Both
};

class FormulaImplication : public Formula
{
public:
    ImplicationDirection direction;
    std::shared_ptr<Formula> left;
    std::shared_ptr<Formula> right;

    FormulaImplication(ImplicationDirection direction, std::shared_ptr<Formula> left, std::shared_ptr<Formula> right) :
        direction(direction),
        left(left),
        right(right)
    { }


    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) override
    {
        return accept(this, v);
    }


    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) const override
    {
        return accept(this, v);
    }

private:
    template <typename T>
    static std::shared_ptr<Formula> accept(T* t, FormulaVisitor& v)
    {
        v.before(t);
        acceptAndSwap(t->left, v);
        v.between(t);
        acceptAndSwap(t->right, v);
        return v.after(t);
    }
};

class FormulaLiteral : public Formula
{
public:
    Literal literal;

    FormulaLiteral(const Literal& literal) : literal(literal) {}

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) const override
    {
        return accept(this, v);
    }

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) override
    {
        return accept(this, v);
    }

private:
    template <typename T>
    static std::shared_ptr<Formula> accept(T* t, FormulaVisitor& v)
    {
        v.before(t);
        return v.after(t);
    }
};

enum Quantifier
{
    Universal,  // for all
    Existential // for some
};

class FormulaQuantifier : public Formula
{
public:
    Quantifier quantifier;
    std::vector<Identifier*> variables;
    std::shared_ptr<Formula> child;

    FormulaQuantifier(Quantifier quantifier, const std::vector<Identifier*> variables, const std::shared_ptr<Formula> child) :
        quantifier(quantifier),
        variables(std::move(variables)),
        child(std::move(child))
    {}

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) const override
    {
        return accept(this, v);
    }

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) override
    {
        return accept(this, v);
    }

private:
    template <typename T>
    static std::shared_ptr<Formula> accept(T* t, FormulaVisitor& v)
    {
        v.before(t);
        acceptAndSwap(t->child, v);
        return v.after(t);
    }
};

class FormulaNegation : public Formula
{
public:
    std::shared_ptr<Formula> child;

    FormulaNegation(std::shared_ptr<Formula> child) :
        child(std::move(child))
    {}

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) override
    {
        return accept(this, v);
    }

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) const override
    {
        return accept(this, v);
    }

private:
    template <typename T>
    static std::shared_ptr<Formula> accept(T* t, FormulaVisitor& v)
    {
        v.before(t);
        acceptAndSwap(t->child, v);
        return v.after(t);
    }
};


inline void FormulaVisitor::before(const FormulaJunction * op) { before(static_cast<const Formula *>(op)); }
inline void FormulaVisitor::before(const FormulaNegation * op) { before(static_cast<const Formula *>(op)); }
inline void FormulaVisitor::before(const FormulaLiteral * op) { before(static_cast<const Formula *>(op)); }
inline void FormulaVisitor::before(const FormulaQuantifier * op) { before(static_cast<const Formula *>(op)); }
inline void FormulaVisitor::before(const FormulaImplication * op) { before(static_cast<const Formula *>(op)); }
inline void FormulaVisitor::before(const Formula * op) {};

inline void FormulaVisitor::between(const FormulaJunction * op) { between(static_cast<const Formula *>(op)); }
inline void FormulaVisitor::between(const FormulaNegation * op) { between(static_cast<const Formula *>(op)); }
inline void FormulaVisitor::between(const FormulaLiteral * op) { between(static_cast<const Formula *>(op)); }
inline void FormulaVisitor::between(const FormulaQuantifier * op) { between(static_cast<const Formula *>(op)); }
inline void FormulaVisitor::between(const FormulaImplication * op) { between(static_cast<const Formula *>(op)); }
inline void FormulaVisitor::between(const Formula * op) {};

inline std::shared_ptr<Formula> FormulaVisitor::after(const FormulaJunction* op) { return after(static_cast<const Formula*>(op)); }
inline std::shared_ptr<Formula> FormulaVisitor::after(const FormulaNegation* op) { return after(static_cast<const Formula*>(op)); }
inline std::shared_ptr<Formula> FormulaVisitor::after(const FormulaLiteral* op) { return after(static_cast<const Formula*>(op)); }
inline std::shared_ptr<Formula> FormulaVisitor::after(const FormulaQuantifier* op) { return after(static_cast<const Formula*>(op)); }
inline std::shared_ptr<Formula> FormulaVisitor::after(const FormulaImplication* op) { return after(static_cast<const Formula*>(op)); }
inline std::shared_ptr<Formula> FormulaVisitor::after(const Formula* op) { std::cout << "FOOO" << std::endl; return nullptr; }



class FormulaPrinter : public FormulaVisitor
{
private:
    enum class Type
    {
        Atomic,
        Unary,
        Quantified,
        Binary,
        Undefined
    };

    std::vector<bool> bracketed;
    std::vector<Type> types;
    std::ostream& stream;

public:

    FormulaPrinter(std::ostream& stream) :
        stream(stream),
        types(),
        bracketed()
    {
        types.push_back(Type::Undefined);
        //bracketed.push_back(true);
    }

    inline Type type(const FormulaJunction* _) { return Type::Binary; }
    inline Type type(const FormulaImplication* _) { return Type::Binary; }
    inline Type type(const FormulaNegation* _) { return Type::Unary; }
    inline Type type(const FormulaQuantifier* _) { return Type::Quantified; }
    inline Type type(const FormulaLiteral* _) { return Type::Atomic; }

    inline bool needsBrackets(Type parent, Type child)
    {
        return (child == Type::Binary)
            || (child == Type::Unary && parent == Type::Unary)
            || (child == Type::Quantified && parent == Type::Unary)
            || (parent == Type::Undefined)
            ;
    }

    template <typename T>
    inline void openSubterm(const T* formula)
    {
        Type t = type(formula);
        bool b = needsBrackets(types.back(), t);
        
        if (b)
        {
            stream << "(";
        }

        types.push_back(t);
        bracketed.push_back(b);
    }

    inline void closeSubterm()
    {
        if (bracketed.back())
        {
            stream << ")";
        }
        bracketed.pop_back();
        types.pop_back();
    }


    inline void virtual before(const FormulaJunction * op) override
    {
        openSubterm(op);
    }

    inline virtual void before(const FormulaNegation * op) override
    {
        openSubterm(op);
        stream << "~ ";
    }

    inline virtual void before(const FormulaLiteral * op) override
    {
        openSubterm(op);
        stream << op->literal;
    }

    inline virtual void before(const FormulaQuantifier * op) override
    {
        openSubterm(op);
        switch (op->quantifier)
        {
            case Quantifier::Universal:
                stream << "!";
                break;
            case Quantifier::Existential:
                stream << "?";
                break;
        }
        stream << " [" << join(op->variables, ",") << "] : ";
    }

    inline virtual void before(const FormulaImplication * op) override
    {
        openSubterm(op);
    }

    inline virtual void between(const FormulaJunction * op) override
    {
        switch (op->operation)
        {
            case Operation::Conjunction:
                stream << " & ";
                break;
            case Operation::Disjunction:
                stream << " | ";
                break;
        }
    }

    inline virtual void between(const FormulaImplication * op) override
    {        
        switch (op->direction)
        {
            case ImplicationDirection::Left:
                stream << " <= ";
                break;
            case ImplicationDirection::Right:
                stream << " => ";
                break;
            case ImplicationDirection::Both:
                stream << " <=> ";
                break;
        }
    }

    inline virtual std::shared_ptr<Formula> after(const Formula* op) override
    {
        closeSubterm();
        return nullptr;
    }

};

inline std::ostream& operator<< (std::ostream& stream, const Formula& formula)
{
    FormulaPrinter p(stream);
    formula.accept(p);
    return stream;
}

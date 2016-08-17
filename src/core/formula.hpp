#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <unordered_map>

namespace theseus
{
 
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

    class Term
    {
    public:
        Identifier* identifier;
        std::vector<Term*> children;

        Term(Identifier* identifier) :
            identifier(identifier),
            children()
        { }
    };

    inline std::ostream& operator<< (std::ostream& stream, const Term& term)
    {
        stream << *term.identifier;
        if (!term.children.empty())
        {
            stream << "(";
            int index = 0;
            for (auto& child : term.children)
            {
                if (index++ > 0)
                    stream << ", ";
                stream << *child;
            }
            stream << ")";
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

    class Formula
    {
    protected:
        virtual std::ostream& print(std::ostream& stream) const = 0;
    public:
        virtual ~Formula() {}
        virtual std::shared_ptr<Formula> accept(FormulaVisitor*) = 0;

        friend std::ostream& operator<< (std::ostream& stream, const Formula& formula)
        {
            return formula.print(stream);
        }
    };

    

    class FormulaJunction;
    class FormulaNegation;
    class FormulaLiteral;
    class FormulaQuantifier;




    class FormulaVisitor
    {
    public:
        virtual void before(FormulaJunction* const op) { }
        virtual void before(FormulaNegation* const op) { }
        virtual void before(FormulaLiteral* const op) { }
        virtual void before(FormulaQuantifier* const op) { }

        virtual std::shared_ptr<Formula> after(FormulaJunction* op) { return nullptr; }
        virtual std::shared_ptr<Formula> after(FormulaNegation* op) { return nullptr; }
        virtual std::shared_ptr<Formula> after(FormulaLiteral* op) { return nullptr; }
        virtual std::shared_ptr<Formula> after(FormulaQuantifier* op) { return nullptr; }
    };



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


        virtual std::shared_ptr<Formula> accept(FormulaVisitor* v) override
        {
            v->before(this);
            for (auto& c : children)
            {
                auto result = c->accept(v);
                if (result)
                    c = std::move(result);
            }
            return std::move(v->after(this));
        }

    protected:
        virtual std::ostream& print(std::ostream& stream) const override
        {
            stream << "(" << *(children.front()) << " " << std::flush;
            switch (operation)
            {
                case Operation::Conjunction:
                    stream << "&";
                    break;
                case Operation::Disjunction:
                    stream << "|";
                    break;
            }
            stream << *(children.back()) << ")" << std::flush;;
            return stream;
        }
    };

    class FormulaLiteral : public Formula
    {
    public:
        Literal literal;

        FormulaLiteral(const Literal& literal) : literal(literal) {}

        virtual std::shared_ptr<Formula> accept(FormulaVisitor* v) override
        {
            v->before(this);
            return std::move(v->after(this));
        }

    protected:
        virtual std::ostream& print(std::ostream& stream) const override
        {
            return stream << literal;
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

        virtual std::shared_ptr<Formula> accept(FormulaVisitor* v) override
        {
            v->before(this);
            auto result = child->accept(v);
            if (result)
                child = std::move(result);
            return std::move(v->after(this));
        }

    protected:
        virtual std::ostream& print(std::ostream& stream) const override
        {
            switch (quantifier)
            {
                case Quantifier::Universal:
                    stream << "forall";
                    break;
                case Quantifier::Existential:
                    stream << "exists";
                    break;
            }
            stream << "(";

            int index = 0;
            for (auto& var : variables)
            {
                if (index++ > 0)
                    stream << ", ";
                stream << *var;
            }
            stream << ")(" << *child << ")";
            return stream;
        }
    };

    class FormulaNegation : public Formula
    {
    public:
        std::shared_ptr<Formula> child;

        FormulaNegation(std::shared_ptr<Formula> child) :
            child(std::move(child))
        {}

        virtual std::shared_ptr<Formula> accept(FormulaVisitor* v) override
        {
            v->before(this);
            auto result = child->accept(v);
            if (result)
                child = std::move(result);
            return std::move(v->after(this));
        }

    protected:
        virtual std::ostream& print(std::ostream& stream) const override
        {
            return stream << "~" << *child;
        }
    };

}

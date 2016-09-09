#include "cop/normalizer.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "core/formula.hpp"
#include "core/identifier.hpp"
#include "core/term.hpp"
#include "core/visitor.hpp"

using namespace std;

  
class ImplicationReplacer : public FormulaVisitor
{
public:
    virtual shared_ptr<Formula> after(const FormulaImplication* op) override
    {
        switch (op->direction)
        {
            // (a => b) === (-a | b)
            case ImplicationDirection::Right:
                return make_shared<FormulaJunction>(Operation::Disjunction,
                    vector<shared_ptr<Formula>>({
                        make_shared<FormulaNegation>(op->left),
                        op->right
                    }));

            // (a <= b) === (a | -b)
            case ImplicationDirection::Left:
                return make_shared<FormulaJunction>(Operation::Disjunction,
                    vector<shared_ptr<Formula>>({
                        op->left,
                        make_shared<FormulaNegation>(op->right)
                    }));

            // (a <=> b) === (a => b) & (a <= b) === (-a | b) & (a | -b)
            // (b & a) | (-a & -b)
            case ImplicationDirection::Both:
                return make_shared<FormulaJunction>(Operation::Conjunction,
                    vector<shared_ptr<Formula>>({

                        make_shared<FormulaJunction>(Operation::Disjunction,
                        vector<shared_ptr<Formula>>({
                            make_shared<FormulaNegation>(op->left),
                            op->right
                        })),

                        make_shared<FormulaJunction>(Operation::Disjunction,
                        vector<shared_ptr<Formula>>({
                            op->left,
                            make_shared<FormulaNegation>(op->right)
                        }))
                    }));
        };
    }
};

class NegationPusher : public FormulaVisitor
{
private:
    bool inverted = false;
public:

    virtual void before(const FormulaImplication* op) override
    {
        throw logic_error("Negation Pusher expects already resolved implications.");
    }

    virtual void before(const FormulaNegation* op) override
    {
        inverted = !inverted;
    }

    virtual shared_ptr<Formula> after(const FormulaNegation* op) override
    {
        inverted = !inverted;
        return std::move(op->child);
    }

    virtual shared_ptr<Formula> after(const FormulaJunction* op) override
    { 
        if (inverted)
        {
            auto operation = (op->operation == Operation::Conjunction)
                            ? Operation::Disjunction
                            : Operation::Conjunction;

            return make_shared<FormulaJunction>(operation, op->children);
        }
        return nullptr;
    }

    virtual shared_ptr<Formula> after(const FormulaLiteral* op) override
    {
        if (inverted)
        {
            Literal literal(op->literal.term);
            literal.polarity = (op->literal.polarity == Polarity::Positive)
                          ? Polarity::Negative
                          : Polarity::Positive;
            return make_shared<FormulaLiteral>(literal);
        }
        return nullptr;
    }

    virtual shared_ptr<Formula> after(const FormulaQuantifier* op) override
    {
        if (inverted)
        {
            auto quantifier = (op->quantifier == Quantifier::Universal)
                          ? Quantifier::Existential
                          : Quantifier::Universal;
            return make_shared<FormulaQuantifier>(quantifier, op->variables, op->child);
        }
        return nullptr;
    }

};



class Skolemizer : public FormulaVisitor
{
private:
    vector<Term*> context;
    unordered_map<Identifier*, Term*> substitutions;
public:

    virtual void before(const FormulaQuantifier* op) override
    {
        if (op->quantifier == Quantifier::Existential)
        {
            for (auto& v : op->variables)
            {
                Identifier* f = IdentifierFactory::instance().inventVariable(v->symbol);
                Term* t = TermDatabase::instance().get(f);
                substitutions[v] = t;
                context.push_back(t);
            }
        }
        else
        {
            for (auto& v : op->variables)
            {
                Identifier* f = IdentifierFactory::instance().inventFunction("c");
                Term* t = TermDatabase::instance().get(f, context);
                substitutions[v] = t;
            }
        }
    }

    virtual shared_ptr<Formula> after(const FormulaLiteral* op) override
    {
        Literal literal = op->literal;
        for (auto& s : substitutions)
        {
            literal.term = TermDatabase::instance().substitute(literal.term, s.first, s.second);    
        }
        return make_shared<FormulaLiteral>(literal);
    }

    virtual shared_ptr<Formula> after(const FormulaQuantifier* op) override
    {
        if (op->quantifier == Quantifier::Existential)
        {
            for (auto& v : op->variables)
            {
                //substitutions.erase(v);
                context.pop_back();
            }
        }
        else
        {
            for (auto& v : op->variables)
            {
                //substitutions.erase(v);
            }
        }
        return std::move(op->child);
    }

};



class NormalFormer : public FormulaVisitor
{
private:
    vector<unordered_map<const Formula*, const FormulaJunction*>> nested;
    Operation major;
    Operation minor;
    bool stable;
public:

    NormalFormer(Operation major) :
        major(major),
        minor((major == Operation::Conjunction) ? Operation::Disjunction : Operation::Conjunction),
        stable(true)
    {
        nested.emplace_back();
    }

    virtual void before(const Formula* op) override
    {
        nested.emplace_back();
    }

    virtual shared_ptr<Formula> after(const Formula* op) override
    {
        nested.pop_back();
        return nullptr;
    }

    virtual shared_ptr<Formula> after(const FormulaJunction* op) override
    {
        if (op->operation == major)
        {
            nested.pop_back();
            nested.back()[op] = op;
            return nullptr;
        }
        else //(op->operation == Operation::Disjunction)
        {
            vector<shared_ptr<FormulaJunction>> newChildren;
            newChildren.emplace_back(make_shared<FormulaJunction>(minor, vector<shared_ptr<Formula>>()));
            for (auto& child : op->children)
            {
                auto found = nested.back().find(child.get());
                if (found == nested.back().end())
                {
                    for (auto& newChild : newChildren)
                    {
                        newChild->children.emplace_back(child);
                    }
                }
                else
                {
                    vector<shared_ptr<FormulaJunction>> copiedNewChildren;
                    for (auto& newChild : newChildren)
                    {
                        for (auto& grandChild : found->second->children)
                        {
                            auto copied = newChild->children;
                            copied.emplace_back(grandChild);
                            copiedNewChildren.emplace_back(make_shared<FormulaJunction>(minor, copied));
                        }
                        if (found->second->children.size() > 1)
                        {
                            stable = false;
                        }
                    }
                    newChildren = move(copiedNewChildren);
                }
            }

            vector<shared_ptr<Formula>> newFormulaChildren;
            for (auto& child : newChildren)
            {
                newFormulaChildren.emplace_back(child);
            }

            auto result = make_shared<FormulaJunction>(major, move(newFormulaChildren));
            nested.pop_back();
            nested.back()[result.get()] = result.get();
            return result;
        }
    }

    bool isStable() const
    {
        return stable;
    }

};


class Flattener : public FormulaVisitor
{
private:
    vector<unordered_map<const Formula*, const FormulaJunction*>> nested;
public:

    Flattener()
    {
        nested.emplace_back();
    }

    virtual void before(const Formula* op) override
    {
        nested.emplace_back();
    }

    virtual shared_ptr<Formula> after(const Formula* op) override
    {
        nested.pop_back();
        return nullptr;
    }

    virtual shared_ptr<Formula> after(const FormulaJunction* op) override
    {
        vector<shared_ptr<Formula>> newChildren;
        for (auto& child : op->children)
        {
            auto found = nested.back().find(child.get());
            if (found != nested.back().end() && found->second->operation == op->operation)
            {
                for (auto& grandChild : found->second->children)
                {
                    newChildren.emplace_back(grandChild);
                }
            }
            else
            {
                newChildren.emplace_back(child);
            }
        }

        auto result = make_shared<FormulaJunction>(op->operation, move(newChildren));
        nested.pop_back();
        nested.back()[result.get()] = result.get();
        return result;
    }

};



class DeEmptifier : public FormulaVisitor
{
public:

    virtual shared_ptr<Formula> after(const FormulaJunction* op) override
    {
        if (op->children.size() == 1)
        {
            return op->children.front();
        }
        else
        {
            return nullptr;
        }
    }

};

shared_ptr<Formula> nonNull(shared_ptr<Formula>&& a, shared_ptr<Formula>&& b)
{
    if (a)
        return a;
    else
        return b;
}

shared_ptr<Formula> normalize(shared_ptr<Formula> f, bool clausal)
{    
    {
        ImplicationReplacer x;
        f = nonNull(f->accept(x), move(f));
    }
 
    {
        NegationPusher x;
        f = nonNull(f->accept(x), move(f));
    }
 
    {
        Skolemizer x;
        f = nonNull(f->accept(x), move(f));
    }

    while (clausal)
    {
        NormalFormer x(Operation::Disjunction);
        f = nonNull(f->accept(x), move(f)); 
        if (x.isStable())
        {
            break;
        }
    }

    {
        DeEmptifier x;
        f = nonNull(f->accept(x), move(f)); 
    }

    {
        f = make_shared<FormulaJunction>(Operation::Disjunction, vector<shared_ptr<Formula>>(1, f)); 
    }

    {
        Flattener x;
        f = nonNull(f->accept(x), move(f));   
    }

    return f;
}

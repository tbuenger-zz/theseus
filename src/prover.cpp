#include <iostream>
#include <vector>
#include <unordered_map>
#include "formula.h"

using namespace std;

namespace theseus
    {

    class NegationPusher : public FormulaVisitor
    {
    private:
        bool inverted = false;
    public:

        virtual void before(FormulaNegation* const op) override
        {
            inverted = !inverted;
        }

        virtual shared_ptr<Formula> after(FormulaNegation* op) override
        {
            inverted = !inverted;
            return std::move(op->child);
        }

        virtual shared_ptr<Formula> after(FormulaJunction* op) override
        { 
            if (inverted)
            {
                op->operation = (op->operation == Operation::Conjunction)
                                ? Operation::Disjunction
                                : Operation::Conjunction;
            }
            return nullptr;
        }

        virtual shared_ptr<Formula> after(FormulaLiteral* op) override
        {
            if (inverted)
            {
                op->literal.polarity = (op->literal.polarity == Polarity::Positive)
                              ? Polarity::Negative
                              : Polarity::Positive;
            }
            return nullptr;
        }

        virtual shared_ptr<Formula> after(FormulaQuantifier* op) override
        {
            if (inverted)
            {
                op->quantifier = (op->quantifier == Quantifier::Universal)
                              ? Quantifier::Existential
                              : Quantifier::Universal;
            }
            return nullptr;
        }

    };



    class Skolemizer : public FormulaVisitor
    {
    private:
        std::vector<Symbol*> context;
        std::unordered_map<Symbol*, Term*> substitutions;
    public:

        virtual void before(FormulaQuantifier* const op) override
        {
            if (op->quantifier == Quantifier::Universal)
            {
                for (auto& v : op->variables)
                    context.push_back(v);
            }
            else
            {
                std::vector<Term*> children;
                std::transform(std::begin(context), std::end(context), std::back_inserter(children),
                    [](Symbol* v){ return TermDatabase::get().get(v);});
                for (auto& v : op->variables)
                {
                    Symbol* f = SymbolDatabase::get().function("$", context.size());
                    Term* t = TermDatabase::get().get(f, children);
                    substitutions[v] = t;
                }
            }
        }

        virtual shared_ptr<Formula> after(FormulaLiteral* op) override
        {
            for (auto& s : substitutions)
            {
                op->literal.term = TermDatabase::get().substitute(op->literal.term, s.first, s.second);    
            }
            
            return nullptr;
        }

        virtual shared_ptr<Formula> after(FormulaQuantifier* op) override
        {
            if (op->quantifier == Quantifier::Universal)
            {
                for (auto& v : op->variables)
                    context.pop_back();
            }
            else
            {
                for (auto& v : op->variables)
                    substitutions.erase(v);
            }
            return std::move(op->child);
        }

    };

    class Clause
    {
    public:
        std::vector<Literal> literals;
    };

    class Matrix
    {
    public:
        std::vector<Clause> clauses;
    };

}

int main()
{
    using namespace theseus;
    auto x = SymbolDatabase::get().variable("x");
    auto y = SymbolDatabase::get().variable("y");
    auto a = SymbolDatabase::get().function("a", 0);
    auto b = SymbolDatabase::get().function("b", 0);
    auto f = SymbolDatabase::get().function("f", 1);
    auto g = SymbolDatabase::get().function("g", 2);
    auto P = SymbolDatabase::get().function("P", 1);
    auto Q = SymbolDatabase::get().function("Q", 2);

    auto tx = TermDatabase::get().get(x);
    auto ty = TermDatabase::get().get(y);
    auto ta = TermDatabase::get().get(a);
    auto tb = TermDatabase::get().get(b);
    auto tfx = TermDatabase::get().get(f, vector<Term*>{tx});
    auto tgx = TermDatabase::get().get(g, vector<Term*>{ta,ty});
    auto tq = TermDatabase::get().get(Q, vector<Term*>{tfx, tgx});

    auto tg2 = TermDatabase::get().get(g, vector<Term*>{ty, tb});
    auto tp = TermDatabase::get().get(P, vector<Term*>{tg2});


    shared_ptr<FormulaLiteral> f1(new FormulaLiteral());
    f1->literal.term = tp;

    shared_ptr<FormulaLiteral> f2(new FormulaLiteral());
    f2->literal.term = tq;

    shared_ptr<FormulaJunction> f3(new FormulaJunction());
    f3->operation = Operation::Conjunction;
    f3->children.push_back(std::move(f1));
    f3->children.push_back(std::move(f2));


    shared_ptr<FormulaNegation> f4(new FormulaNegation());
    f4->child = std::move(f3);

    shared_ptr<Formula> ff = std::move(f4);



    NegationPusher n;
    Skolemizer s;


    std::cout << ff->to_string() << std::endl;

    auto ff2 = std::move(ff->accept(&n));
    if (ff2)
        ff = std::move(ff2);

    std::cout << ff->to_string() << std::endl;


    return 0;
}
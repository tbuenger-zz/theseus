#include "core/formula.hpp"
#include "core/printer.hpp"

using namespace std;

namespace
{

    template <typename T>
    void acceptAndSwap(shared_ptr<T>& a, FormulaVisitor& v)
    {
        auto result = a->accept(v);
        if (result)
            a = move(result);
    }


    template <typename T>
    void acceptAndSwap(const shared_ptr<T>& a, FormulaVisitor& v)
    {
        auto result = a->accept(v);
        if (result)
            throw invalid_argument("Visitor should not modify");
    }


    template <typename T>
    static shared_ptr<Formula> acceptJunction(T* t, FormulaVisitor& v)
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


    template <typename T>
    static shared_ptr<Formula> acceptImplication(T* t, FormulaVisitor& v)
    {
        v.before(t);
        acceptAndSwap(t->left, v);
        v.between(t);
        acceptAndSwap(t->right, v);
        return v.after(t);
    }


    template <typename T>
    static shared_ptr<Formula> acceptLiteral(T* t, FormulaVisitor& v)
    {
        v.before(t);
        return v.after(t);
    }


    template <typename T>
    static shared_ptr<Formula> acceptQuantifier(T* t, FormulaVisitor& v)
    {
        v.before(t);
        acceptAndSwap(t->child, v);
        return v.after(t);
    }


    template <typename T>
    static shared_ptr<Formula> acceptNegation(T* t, FormulaVisitor& v)
    {
        v.before(t);
        acceptAndSwap(t->child, v);
        return v.after(t);
    }

}


shared_ptr<Formula> FormulaJunction::accept(FormulaVisitor& v)
{
    return acceptJunction(this, v);
}


shared_ptr<Formula> FormulaJunction::accept(FormulaVisitor& v) const
{
    return acceptJunction(this, v);
}


shared_ptr<Formula> FormulaImplication::accept(FormulaVisitor& v)
{
    return acceptImplication(this, v);
}


shared_ptr<Formula> FormulaImplication::accept(FormulaVisitor& v) const
{
    return acceptImplication(this, v);
}


shared_ptr<Formula> FormulaLiteral::accept(FormulaVisitor& v) const
{
    return acceptLiteral(this, v);
}


shared_ptr<Formula> FormulaLiteral::accept(FormulaVisitor& v)
{
    return acceptLiteral(this, v);
}


shared_ptr<Formula> FormulaQuantifier::accept(FormulaVisitor& v) const
{
    return acceptQuantifier(this, v);
}


shared_ptr<Formula> FormulaQuantifier::accept(FormulaVisitor& v)
{
    return acceptQuantifier(this, v);
}


shared_ptr<Formula> FormulaNegation::accept(FormulaVisitor& v)
{
    return acceptNegation(this, v);
}


shared_ptr<Formula> FormulaNegation::accept(FormulaVisitor& v) const
{
    return acceptNegation(this, v);
}


ostream& operator<< (ostream& stream, const Formula& formula)
{
    FormulaPrinter p(stream);
    formula.accept(p);
    return stream;
}

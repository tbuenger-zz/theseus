#pragma once

#include <iosfwd>
#include <vector>
#include "core/visitor.hpp"

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
    bool debug;

public:

    FormulaPrinter(std::ostream& stream, bool debug=false) :
        stream(stream),
        types(1, Type::Undefined),
        bracketed(),
        debug(debug)
    { }

    inline Type type(const FormulaJunction* _) { return Type::Binary; }
    inline Type type(const FormulaImplication* _) { return Type::Binary; }
    inline Type type(const FormulaNegation* _) { return Type::Unary; }
    inline Type type(const FormulaQuantifier* _) { return Type::Quantified; }
    inline Type type(const FormulaLiteral* _) { return Type::Atomic; }

    bool needsBrackets(Type parent, Type child);

    template <typename T>
    inline void openSubterm(const T* formula);

    inline void closeSubterm();

    virtual void before(const FormulaJunction * op) override;
    virtual void before(const FormulaNegation * op) override;
    virtual void before(const FormulaLiteral * op) override;
    virtual void before(const FormulaQuantifier * op) override;
    virtual void before(const FormulaImplication * op) override;

    virtual void between(const FormulaJunction * op) override;
    virtual void between(const FormulaImplication * op) override;

    virtual std::shared_ptr<Formula> after(const Formula* op) override;
};
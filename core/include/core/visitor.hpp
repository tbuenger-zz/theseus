#pragma once

#include <memory>
#include "core/formula.hpp"


class FormulaVisitor
{
public:
 
    virtual void before(const FormulaJunction * op) { before(static_cast<const Formula *>(op)); }
    virtual void before(const FormulaNegation * op) { before(static_cast<const Formula *>(op)); }
    virtual void before(const FormulaLiteral * op) { before(static_cast<const Formula *>(op)); }
    virtual void before(const FormulaQuantifier * op) { before(static_cast<const Formula *>(op)); }
    virtual void before(const FormulaImplication * op) { before(static_cast<const Formula *>(op)); }
    virtual void before(const Formula * op) { }

    virtual void between(const FormulaJunction * op) { between(static_cast<const Formula *>(op)); }
    virtual void between(const FormulaNegation * op) { between(static_cast<const Formula *>(op)); }
    virtual void between(const FormulaLiteral * op) { between(static_cast<const Formula *>(op)); }
    virtual void between(const FormulaQuantifier * op) { between(static_cast<const Formula *>(op)); }
    virtual void between(const FormulaImplication * op) { between(static_cast<const Formula *>(op)); }
    virtual void between(const Formula * op) { }

    virtual std::shared_ptr<Formula> after(const FormulaJunction* op) { return after(static_cast<const Formula*>(op)); }
    virtual std::shared_ptr<Formula> after(const FormulaNegation* op) { return after(static_cast<const Formula*>(op)); }
    virtual std::shared_ptr<Formula> after(const FormulaLiteral* op) { return after(static_cast<const Formula*>(op)); }
    virtual std::shared_ptr<Formula> after(const FormulaQuantifier* op) { return after(static_cast<const Formula*>(op)); }
    virtual std::shared_ptr<Formula> after(const FormulaImplication* op) { return after(static_cast<const Formula*>(op)); }
    virtual std::shared_ptr<Formula> after(const Formula* op) { return nullptr; }

};

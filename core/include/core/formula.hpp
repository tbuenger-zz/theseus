#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iosfwd>

#include "core/utils.hpp"
#include "core/literal.hpp"

class FormulaVisitor;
class Identifier;

class Formula
{
public:
    virtual ~Formula() {}
    virtual std::shared_ptr<Formula> accept(FormulaVisitor&) = 0;
    virtual std::shared_ptr<Formula> accept(FormulaVisitor&) const = 0;

    friend std::ostream& operator<< (std::ostream& stream, const Formula& formula);
    inline std::string toString() const { return (std::ostringstream() << *this).str(); }
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

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) override;
    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) const override;
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

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) override;
    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) const override;
};


class FormulaLiteral : public Formula
{
public:
    Literal literal;

    FormulaLiteral(const Literal& literal) : literal(literal) {}

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) const override;
    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) override;
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

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) const override;
    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) override;
};


class FormulaNegation : public Formula
{
public:
    std::shared_ptr<Formula> child;

    FormulaNegation(std::shared_ptr<Formula> child) :
        child(std::move(child))
    {}

    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) override;
    virtual std::shared_ptr<Formula> accept(FormulaVisitor& v) const override;
};

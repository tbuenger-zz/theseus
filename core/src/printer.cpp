#include "core/printer.hpp"
#include "core/formula.hpp"
#include "core/identifier.hpp"

using namespace std;


bool FormulaPrinter::needsBrackets(Type parent, Type child)
{
    return (child == Type::Binary)
        || (child == Type::Unary && parent == Type::Unary)
        || (child == Type::Quantified && parent == Type::Unary)
        || (parent == Type::Undefined)
        ;
}

const char* debugLabel(const Formula* t) { return "UNKNOWN"; }
const char* debugLabel(const FormulaJunction* t) { return t->operation == Operation::Conjunction ? "AND" : "OR"; }
const char* debugLabel(const FormulaNegation* t) { return "Neg"; }
const char* debugLabel(const FormulaLiteral* t) { return "Lit"; }
const char* debugLabel(const FormulaImplication* t) { return "Impl"; }
const char* debugLabel(const FormulaQuantifier* t) { return "Quant"; }

template <typename T>
void FormulaPrinter::openSubterm(const T* formula)
{
    if (debug)
    {
        stream << "[" << debugLabel(formula) << "]";
    }
    Type t = type(formula);
    bool b = needsBrackets(types.back(), t);
    
    if (b)
    {
        stream << "(";
    }

    types.push_back(t);
    bracketed.push_back(b);
}


template void FormulaPrinter::openSubterm(const FormulaJunction*);
template void FormulaPrinter::openSubterm(const FormulaNegation*);
template void FormulaPrinter::openSubterm(const FormulaLiteral*);
template void FormulaPrinter::openSubterm(const FormulaQuantifier*);
template void FormulaPrinter::openSubterm(const FormulaImplication*);

void FormulaPrinter::closeSubterm()
{
    if (bracketed.back())
    {
        stream << ")";
    }
    bracketed.pop_back();
    types.pop_back();
}


void FormulaPrinter::before(const FormulaJunction * op)
{
    openSubterm(op);
}

void FormulaPrinter::before(const FormulaNegation * op)
{
    openSubterm(op);
    stream << "~ ";
}

void FormulaPrinter::before(const FormulaLiteral * op)
{
    openSubterm(op);
    stream << op->literal;
}

void FormulaPrinter::before(const FormulaQuantifier * op)
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

void FormulaPrinter::before(const FormulaImplication * op)
{
    openSubterm(op);
}

void FormulaPrinter::between(const FormulaJunction * op)
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

void FormulaPrinter::between(const FormulaImplication * op)
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

shared_ptr<Formula> FormulaPrinter::after(const Formula* op)
{
    closeSubterm();
    return nullptr;
}
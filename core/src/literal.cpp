#include "core/literal.hpp"
#include "core/term.hpp"

std::ostream& operator<< (std::ostream& stream, const Literal& literal)
{
    if (literal.polarity == Polarity::Negative)
        stream << "~";
    stream << *literal.term;
    return stream;
}
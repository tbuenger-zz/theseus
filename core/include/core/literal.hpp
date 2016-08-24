#pragma once

#include <iosfwd>

class Term;

enum class Polarity
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


std::ostream& operator<< (std::ostream& stream, const Literal& literal);
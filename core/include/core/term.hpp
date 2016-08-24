#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iosfwd>
#include <unordered_set>

#include "core/utils.hpp"


class Identifier;


class Term
{
public:
    Identifier* identifier;
    std::vector<Term*> children;

    Term(Identifier* identifier) :
        identifier(identifier),
        children()
    { }

    std::string toString() const { return (std::ostringstream() << *this).str(); }
};


std::ostream& operator<< (std::ostream& stream, const Term& term);


class TermHasher
{   
public:
    // hash
    size_t operator()(const std::unique_ptr<Term>& t) const;
    // equal
    bool operator()(const std::unique_ptr<Term>& lhs, const std::unique_ptr<Term>& rhs ) const;
};


class TermDatabase : public Singleton<TermDatabase>
{
private:
    std::unordered_set<std::unique_ptr<Term>, TermHasher, TermHasher> known;
    std::vector<std::unique_ptr<Term>> heap;

    Term* singelize(std::unique_ptr<Term> term);

public:

    Term* get(Identifier* identifier);
    Term* get(Identifier* identifier, std::vector<Term*> children);

    Term* substitute(Term* root, Identifier* needle, Term* replacement);
    Term* substitute(Term* root, Term* needle, Term* replacement);
};
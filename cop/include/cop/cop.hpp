#pragma once

#include "core/term.hpp"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iosfwd>

class NotUnifyableException : public std::logic_error {
  public:
    NotUnifyableException(const std::string& msg) : std::logic_error(msg) { }
    NotUnifyableException(const char *msg) : std::logic_error(msg) { }
    ~NotUnifyableException() throw() { }
};

class SelfContainmentException : public std::logic_error {
  public:
    SelfContainmentException(const std::string& msg) : std::logic_error(msg) { }
    SelfContainmentException(const char *msg) : std::logic_error(msg) { }
    ~SelfContainmentException() throw() { }
};

class Unifier
{
public:

    struct Node
    {
        Term* term;
        size_t depth;

        Node() : term(nullptr), depth(0) { }
    };

    std::unordered_map<Term*, Node> parents;

    void unionTrees(Term* a, Term* b);

    Term* find(Term* a);

    void validate();
    void validLoop(Term*, std::unordered_set<Term*>&, bool);

public:
    Term* resolve(Identifier*);
    Term* resolve(Term*);

    void unify(Term* a, Term* b);
    void unifyRecursive(Term* a, Term* b);
};

std::ostream& operator<<(std::ostream& os, const Unifier& u);
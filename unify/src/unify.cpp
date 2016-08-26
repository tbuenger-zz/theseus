#include "unify/unify.hpp"
#include "core/identifier.hpp"
#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>
#include <sstream>

using namespace std;

namespace
{
    ostream& operator<<(ostream& os, const Term* t)
    {
        if (t)
            os << *t;
        else
            os << "nullptr";
        return os;
    }
}

ostream& operator<<(ostream& os, const Unifier& u)
{
    os << "parents:" << endl;
    for(const auto& p : u.parents)
    {
        os << " - " << p.first << " => " << p.second.term << endl;
    }
    return os;
}

void Unifier::unify(Term* aInit, Term* bInit)
{
    unifyRecursive(aInit, bInit);
    validate();
}

/*
void Unifier::validate()
{
    unordered_set<Term*> visited;
    unordered_set<Term*> loopVisited;
    vector<Term*> termsToVisit;
    bool includesNonVariable;

    for (auto& pair : parents)
    {
        Term* term = pair.first;
        if (visited.find(term) != visited.end())
        {
            termsToVisit.clear();
            termsToVisit.push_back(term);
            loopVisited.clear();
            includesNonVariable = false;

            do
            {
                term = termsToVisit.back();
                termsToVisit.pop_back();

                Term* parent = find(term);
                if (term->identifier->isVariable)
                {
                    termsToVisit.push_back(parent);
                }
                else
                {
                    includesNonVariable = true;
                    for (Term* child : term->children)
                    {
                        termsToVisit.push_back(child);
                    }
                } 
            } while(loopVisited.insert(term).second && termsToVisit.size() > 0);

            if (includesNonVariable)
            {
                throw SelfContainmentException("");
            }

            for (Term* t : loopVisited)
            {
                visited.insert(t);
            }
        }
    }
}
*/
void Unifier::validate()
{
    unordered_map<Term*, unordered_set<Term*>> roots;
    for (auto& pair : parents)
    {
        roots[find(pair.first)].insert(pair.first);
    }
    for (auto& pair : roots)
    {
        if (!pair.first->identifier->isVariable)
        {
            validLoop(pair.first, pair.second, false);
        }
    }
}

string indentation = "";

void Unifier::validLoop(Term* t, unordered_set<Term*>& visited, bool includesNonVariable)
{
    cout << indentation << "> " << *t << endl;
    if (t->identifier->isVariable)
    {
        bool alreadyVisited = !visited.insert(t).second;
        if (alreadyVisited && includesNonVariable)
        {
            throw SelfContainmentException("Loop including " + t->identifier->symbol);
        }
        indentation += " ";
        Term* parent = find(t);
        if (t != parent)
        {
            validLoop(parent, visited, includesNonVariable);
        }
        indentation = indentation.substr(1);
        if (!alreadyVisited)
        {
            visited.erase(t);
        }
    }
    else
    {
        for (Term* child : t->children)
        {
            indentation += " ";
            validLoop(child, visited, true);
            indentation = indentation.substr(1);
        }
    }
}


unique_ptr<Identifier> LOOPIdentifier = make_unique<Identifier>("LOOP", false);
unique_ptr<Term> LOOP = make_unique<Term>(LOOPIdentifier.get());


void Unifier::unifyRecursive(Term* aInit, Term* bInit)
{
    cout << "unify " << aInit << " = " << bInit << endl;
    Term* a = find(aInit);
    Term* b = find(bInit);
    cout << "now " << a << " = " << b << endl;

    if (a == b)
    {
        return;
    }
    if (a->identifier->isVariable && b->identifier->isVariable)
    {
        unionTrees(a, b);
        cout << "after unify:" << endl << *this << endl;
        return;
    }
    else if (a->identifier->isVariable)
    {
        parents[a].term = b;
        cout << "after unify:" << endl << *this << endl;
        return;
    }
    else if (b->identifier->isVariable)
    {
        parents[b].term = a;
        cout << "after unify:" << endl << *this << endl;
        return;
    }
    else if (a->identifier == b->identifier && a->children.size() == b->children.size())
    {
        if (aInit != a)
            parents[aInit].term = LOOP.get();

        mismatch(a->children.begin(), a->children.end(), b->children.begin(), [this](Term* x, Term* y) {
             unifyRecursive(x,y); return true; });
        cout << "after unify:" << endl << *this << endl;

        if (aInit != a)
            parents[aInit].term = a;

        return;
    }
    throw NotUnifyableException((ostringstream() << *a << "!=" << *b).str());
}


void Unifier::unionTrees(Term* a, Term* b)
{
    Node& aa = parents[a];
    Node& bb = parents[b];
    if (aa.depth < bb.depth)
    {
        aa.term = b;
    }
    else if (aa.depth > bb.depth)
    {
        bb.term = a;
    }
    else
    {
        aa.term = b;
        ++bb.depth;
    }
}


Term* Unifier::find(Term* a)
{
    // without path compression
    Term* parent = a;
    size_t aDepth = 0;
    size_t parentDepth = 0;
    do
    {
        aDepth = parentDepth;
        a = parent;

        if (!a->identifier->isVariable)
            return a;

        auto parentNode = parents[a];

        parent = parentNode.term;
        parentDepth = parentNode.depth;
        //cout << "find: " << a << "[" << aDepth << "]" << " -> " << parent << "[" << parentDepth << "]" << endl;

    } while (parent != nullptr);
    return a;
}


Term* Unifier::resolve(Identifier* i)
{
    return resolve(TermDatabase::instance().get(i));
}

Term* Unifier::resolve(Term* t)
{
    t = find(t);
    if (t->identifier->isVariable)
    {
        return t;
    }
    vector<Term*> newChildren(t->children.size());
    transform(t->children.begin(), t->children.end(), newChildren.begin(), [this](Term* x){ return resolve(x); });
    return TermDatabase::instance().get(t->identifier, std::move(newChildren));
}

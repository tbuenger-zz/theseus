#include "core/term.hpp"
#include "core/identifier.hpp"

using namespace std;


size_t TermHasher::operator()(const unique_ptr<Term>& t) const
{
    size_t hash = 0;
    hash_combine(hash, reinterpret_cast<size_t>(&t));
    for (Term* child : t->children)
    {
        hash_combine(hash, reinterpret_cast<size_t>(child));
    }
    return hash;
}


bool TermHasher::operator()(const unique_ptr<Term>& lhs, const unique_ptr<Term>& rhs ) const
{
    return lhs->identifier == rhs->identifier
        && lhs->children == rhs->children;
}


Term* TermDatabase::singelize(unique_ptr<Term> term)
{
    auto inserted = known.emplace(move(term));
    return inserted.first->get();
}


Term* TermDatabase::get(Identifier* identifier)
{
    return singelize(make_unique<Term>(identifier));
}


Term* TermDatabase::get(Identifier* identifier, vector<Term*> children)
{
    auto result = make_unique<Term>(identifier);
    result->children = children;
    return singelize(move(result));
}


Term* TermDatabase::substitute(Term* root, Identifier* needle, Term* replacement)
{
    return substitute(root, get(needle), replacement);
}


Term* TermDatabase::substitute(Term* root, Term* needle, Term* replacement)
{
    if (root == needle)
        return replacement;
    
    for (size_t i = 0; i<root->children.size(); ++i)
    {
        Term* child = root->children[i];
        Term* new_child = substitute(child, needle, replacement);
        if (new_child != child)
        {
            auto result = make_unique<Term>(root->identifier);
            result->children = root->children;

            result->children[i] = new_child;
            for (size_t k = i+1; k<result->children.size(); ++k)
            {
                result->children[k] = substitute(root->children[k], needle, replacement);
            }

            return singelize(move(result));
        }
    }
    return root;
}


ostream& operator<< (ostream& stream, const Term& term)
{
    stream << *term.identifier;
    if (!term.children.empty())
    {
        stream << "(" << join(term.children, ",") << ")";
    }
    return stream;
}
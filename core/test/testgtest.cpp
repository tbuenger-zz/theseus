#include "gtest/gtest.h"
#include "core/term.hpp"
#include "core/identifier.hpp"
#include <cctype>
#include <initializer_list>
#include <vector>

using namespace std;

namespace
{
    ostream& operator<<(ostream& os, const Term* t)
    {
        if (t)
            os << *t << endl;
        else
            os << "nullptr" << endl;
        return os;
    }
}

Identifier* i(const string& in)
{
    if (isupper(in.front()))
        return IdentifierFactory::instance().variable(in);
    else
        return IdentifierFactory::instance().function(in);
}
Term* t(const string& l, std::initializer_list<Term*> ts)
{
    return TermDatabase::instance().get(i(l), std::vector<Term*>(ts));
}
Term* t(const string& l)
{
    return t(l, {});
}

TEST(identifier, variable_singeltons)
{
    Identifier* first = i("X");
    Identifier* second = i("X");
    EXPECT_EQ(first, second);
}

TEST(identifier, constant_singeltons)
{
    Identifier* first = i("c");
    Identifier* second = i("c");
    EXPECT_EQ(first, second);
}


TEST(term, variable_singletons)
{
    Term* first = t("X");
    Term* second = t("X");
    EXPECT_EQ(first, second);
}

TEST(term, constant_singletons)
{
    Term* first = t("c");
    Term* second = t("c");
    EXPECT_EQ(first, second);
}

TEST(term, predicate_singletons)
{
    Term* a = t("a");
    Term* first = t("p", {a});
    Term* second = t("p", {a});
    EXPECT_EQ(first, second);
}

TEST(term, complexpredicate_singletons)
{
    Term* first = t("p", {t("f", {t("X")}), t("a"), t("c")});
    Term* second = t("p", {t("f", {t("X")}), t("a"), t("c")});
    EXPECT_EQ(first, second);
}

#include "gtest/gtest.h"
#include "unify/unify.hpp"
#include "tptp/problem.hpp"
#include "core/formula.hpp"
#include "core/literal.hpp"
#include "core/identifier.hpp"

using namespace std;


ostream& operator<<(ostream& os, const Term* t)
{
    if (t)
        os << *t << endl;
    else
        os << "nullptr" << endl;
    return os;
}


Term* t(const string& input)
{
    auto formula = tptp::parseFormula("p(" + input + ")");
    return static_cast<FormulaLiteral&>(*formula).literal.term->children.front();
}

TEST(unification, find_simple)
{
    Unifier u;
    Term* a = t("t(X)");
    Term* b = t("t(a)");
    u.unify(a,b);

    EXPECT_EQ(t("a"), u.resolve(t("X")));
}

TEST(unification, find_basic)
{
    Unifier u;
    Term* a = t("t(X,b)");
    Term* b = t("t(a,Y)");
    u.unify(a,b);

    EXPECT_EQ(t("a"), u.resolve(t("X")));
    EXPECT_EQ(t("b"), u.resolve(t("Y")));
}

TEST(unification, find_double)
{
    Unifier u;
    Term* a = t("t(X,Y)");
    Term* b = t("t(a,X)");
    u.unify(a,b);

    EXPECT_EQ(t("a"), u.resolve(t("X")));
    EXPECT_EQ(t("a"), u.resolve(t("Y")));
}

TEST(unification, find_not)
{
    Unifier u;
    Term* a = t("t(X,b,Y)");
    Term* b = t("t(a,Y,X)");
    EXPECT_THROW(u.unify(a,b), NotUnifyableException);
}

TEST(unification, find_basic_loop)
{
    Unifier u;
    Term* a = t("t(X,Y)");
    Term* b = t("t(Y,X)");
    u.unify(a,b);
    EXPECT_EQ(t("Y"), u.resolve(t("X")));
}

TEST(unification, find_loop)
{
    Unifier u;
    Term* a = t("t(X,Y,Z)");
    Term* b = t("t(Y,Z,X)");
    u.unify(a,b);
    EXPECT_EQ(t("Y"), u.resolve(t("X")));
}


TEST(unification, find_complex_loop)
{
    Unifier u;
    Term* a = t("t(f(X),  X  )");
    Term* b = t("t( Y  , f(Y))");
    EXPECT_THROW(u.unify(a,b), SelfContainmentException);
}


TEST(unification, find_super_complex_loop)
{
    Unifier u;
    Term* a = t("t(f(X),  X  , Z)");
    Term* b = t("t( Y  , f(Z), Y)");
    EXPECT_THROW(u.unify(a,b), SelfContainmentException);
}



TEST(unification, test2)
{
    Unifier u;
    Term* a = t("f(X,X,Y)");
    Term* b = t("f(g(Y),g(g(Z)),g(a))");
    u.unify(a,b);
    EXPECT_EQ(t("g(g(a))"), u.resolve(t("X")));
    EXPECT_EQ(t("g(a)"), u.resolve(t("Y")));
    EXPECT_EQ(t("a"), u.resolve(t("Z")));
}




TEST(unification, test3)
{
    Unifier u;
    Term* a = t("f(X, X, Y)");
    Term* b = t("f(X, g(a), g(b))");
    u.unify(a,b);
    EXPECT_EQ(t("g(a)"), u.resolve(t("X")));
    EXPECT_EQ(t("g(b)"), u.resolve(t("Y")));
}


TEST(unification, test4)
{
    Unifier u;
    Term* a = t("f(X,X,Y,Y)");
    Term* b = t("f(g(Y),g(g(Z)),g(a),g(X))");
    EXPECT_THROW(u.unify(a,b), NotUnifyableException);
}

TEST(unification, test5)
{
    Unifier u;
    Term* a = t("q(p(X,Y),p(Y,X))");
    Term* b = t("q(Z,Z)");
    u.unify(a,b);
    EXPECT_EQ(t("X"), u.resolve(t("X")));
    EXPECT_EQ(t("X"), u.resolve(t("Y")));
    EXPECT_EQ(t("p(X,X)"), u.resolve(t("Z")));
}

TEST(unification, test6)
{
    Unifier u;
    Term* a = t("q(X, Y)");
    Term* b = t("q(p(Y,Y), a)");
    u.unify(a,b);
    EXPECT_EQ(t("p(a,a)"), u.resolve(t("X")));
    EXPECT_EQ(t("a"), u.resolve(t("Y")));
}



TEST(unification, test7)
{
    Unifier u;
    Term* a = t("q(g(Y), f(X,   h(X),Y))");
    Term* b = t("q(X   , f(g(Z),A,   Z))");
    u.unify(a,b);
    EXPECT_EQ(t("g(Z)"), u.resolve(t("X")));
    EXPECT_EQ(t("Z"), u.resolve(t("Y")));
    EXPECT_EQ(t("Z"), u.resolve(t("Z")));
    EXPECT_EQ(t("h(g(Z))"), u.resolve(t("A")));
}

TEST(unification, test8)
{
    Unifier u;
    Term* a = t("f(X1, X3, X5, X7, X1, X5, X1) ");
    Term* b = t("f(X2, X4, X6, X8, X3, X7, X5)");
    u.unify(a,b);
    EXPECT_EQ(t("X8"), u.resolve(t("X1")));
    EXPECT_EQ(t("X8"), u.resolve(t("X2")));
    EXPECT_EQ(t("X8"), u.resolve(t("X3")));
    EXPECT_EQ(t("X8"), u.resolve(t("X4")));
    EXPECT_EQ(t("X8"), u.resolve(t("X5")));
    EXPECT_EQ(t("X8"), u.resolve(t("X6")));
    EXPECT_EQ(t("X8"), u.resolve(t("X7")));
    EXPECT_EQ(t("X8"), u.resolve(t("X8")));
}


TEST(unification, test9)
{
    Unifier u;
    Term* a = t("t(X,Y,a)");
    Term* b = t("t(Y,X,X)");
    u.unify(a,b);
    EXPECT_EQ(t("a"), u.resolve(t("X")));
    EXPECT_EQ(t("a"), u.resolve(t("Y")));

}

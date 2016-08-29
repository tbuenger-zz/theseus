#include "gtest/gtest.h"
#include "unify/unify.hpp"
#include "core/formula.hpp"
#include "core/literal.hpp"
#include "core/identifier.hpp"
#include "tptp/problem.hpp"
#include "cop/normalizer.hpp"
#include "core/printer.hpp"


using namespace std;

ostream& operator<<(ostream& os, const Term* t)
{
    if (t)
        os << *t << endl;
    else
        os << "nullptr" << endl;
    return os;
}


shared_ptr<Formula> f(const string& input)
{
    return tptp::parseFormula(input);
}

Term* t(const string& input)
{
    auto formula = tptp::parseFormula("p(" + input + ")");
    return static_cast<FormulaLiteral&>(*formula).literal.term->children.front();
}


class NormalizeTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        IdentifierFactory::instance().clear();
        TermDatabase::instance().clear();
    }
};

TEST_F(NormalizeTest, knn)
{    
    auto f1 = f("p(a) | (q(a) & r(a)) | s(a) | (t(a) & u(a))");

    EXPECT_EQ("((p(a) | q(a) | s(a) | t(a)) & (p(a) | q(a) | s(a) | u(a)) & (p(a) | r(a) | s(a) | t(a)) & (p(a) | r(a) | s(a) | u(a)))", normalize(f1)->toString());
}

TEST_F(NormalizeTest, flatten)
{    
    auto f1 = f("p(a) & (q(a) & r(a)) & s(a) & (t(a) & u(a))");

    EXPECT_EQ("(p(a) & q(a) & r(a) & s(a) & t(a) & u(a))", normalize(f1)->toString());
}

TEST_F(NormalizeTest, complex)
{
    auto f1 = f("! [P,Q,R] : ( ( p(nand(P,nand(Q,R))) & p(P) ) => p(R) ) ");

    EXPECT_EQ("((~p(nand(P_0,nand(Q_0,R_0))) | ~p(P_0) | p(R_0)))", normalize(f1)->toString());
}

TEST_F(NormalizeTest, complex2)
{

    auto f1 = f("( ? [X] :\
        ( algorithm(X)\
        & ! [Y] :\
            ( program(Y)\
           => ! [Z] : decides(X,Y,Z) ) )\
   => ? [W] :\
        ( program(W)\
        & ! [Y] :\
            ( program(Y)\
           => ! [Z] : decides(W,Y,Z) ) ) )");

    EXPECT_EQ("((~algorithm(X_0) | program(Y_0(X_0)) | program(W_0)) & (~algorithm(X_0) | program(Y_0(X_0)) | ~program(Y_1) | decides(W_0,Y_1,Z_1)) & (~algorithm(X_0) | ~decides(X_0,Y_0(X_0),Z_0(X_0)) | program(W_0)) & (~algorithm(X_0) | ~decides(X_0,Y_0(X_0),Z_0(X_0)) | ~program(Y_1) | decides(W_0,Y_1,Z_1)))",
                normalize(f1)->toString());
}

TEST_F(NormalizeTest, simple)
{
    auto f1 = f("p(a) | p(b)");

    EXPECT_EQ("((p(a) | p(b)))", normalize(f1)->toString());
}



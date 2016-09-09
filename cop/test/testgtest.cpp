#include "gtest/gtest.h"
#include "unify/unify.hpp"
#include "core/formula.hpp"
#include "core/literal.hpp"
#include "core/identifier.hpp"
#include "tptp/problem.hpp"
#include "cop/normalizer.hpp"
#include "core/printer.hpp"
#include "cop/cop.hpp"


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
    auto f1 = f("p(a) & (q(a) | r(a)) & s(a) & (t(a) | u(a))");

    EXPECT_EQ("((p(a) & q(a) & s(a) & t(a)) | (p(a) & q(a) & s(a) & u(a)) | (p(a) & r(a) & s(a) & t(a)) | (p(a) & r(a) & s(a) & u(a)))", normalize(f1)->toString());
}

TEST_F(NormalizeTest, flatten)
{    
    auto f1 = f("p(a) | (q(a) | r(a)) | s(a) | (t(a) | u(a))");

    EXPECT_EQ("(p(a) | q(a) | r(a) | s(a) | t(a) | u(a))", normalize(f1)->toString());
}

TEST_F(NormalizeTest, complex)
{
    auto f1 = f("? [P,Q,R] : ( ( p(nand(P,nand(Q,R))) & p(P) ) => p(R) ) ");

    EXPECT_EQ("(~p(nand(P_0,nand(Q_0,R_0))) | ~p(P_0) | p(R_0))", normalize(f1)->toString());
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

    EXPECT_EQ("(~algorithm(c_0) | (program(Y_0) & ~decides(c_0,Y_0,Z_0)) | (program(W_0) & ~program(c_1(W_0))) | (program(W_0) & decides(W_0,c_1(W_0),c_2(W_0))))",
                normalize(f1)->toString());
}

TEST_F(NormalizeTest, simple)
{
    auto f1 = f("p(a) & p(b)");

    EXPECT_EQ("((p(a) & p(b)))", normalize(f1)->toString());
}



TEST(Prover, prove)
{
    // (((∃xQ(x)∨¬Q(c)) ⇒P) ∧ (P ⇒(∃yQ(y)∧R))) ⇒(P∧R) 
    auto problem = f("((( ? [X] : q(X) | ~q(c)) => p) & (p => ( ? [Y] : q(Y) & r))) => (p & r)");

    EXPECT_EQ(true, isTheorem(problem));
}


TEST(Prover, prove2)
{
    // (∀x( (¬P(x)∨Q(f(x))) ⇒ (Q(x)∧(Q(a) ⇒ R(b))∧¬R(x)) )∧Q(f(b))) ⇒ P(a) .
    auto problem = f("( ! [X] : ( (~p(X) | q(f(X))) => (q(X) & (q(a) => r(b)) & ~r(X)) ) & q(f(b))) => p(a)");
    EXPECT_EQ(true, isTheorem(problem));
}



TEST(Prover, prove3)
{
    // P(a)∧(¬((Q(f(f(c)))∧∀x(Q(f(x))⇒Q(x))) ⇒ Q(c))  ∨  ∀y(P(y)⇒P(g(y)))) ⇒ ∃zP(g(g(z)))
    auto problem = f("(p(a) & (~ ((q(f(f(c))) & ! [X] : (q(f(X)) => q(X))) => q(c)) | ! [Y] : (p(Y) => p(g(Y))))) => ? [Z] : p(g(g(Z)))");
    EXPECT_EQ(true, isTheorem(problem));
}



TEST_F(NormalizeTest, expand)
{
    auto f1 = f("(a | (b & c) | d) & (x & y)");
    EXPECT_EQ("((a & x & y) | (b & c & x & y) | (d & x & y))", normalize(f1)->toString());
}


TEST_F(NormalizeTest, provyyy)
{
    auto f1 = f("((a & (~((m & (n => o)) => p) | (b => c))) => d)");
    EXPECT_EQ("(~a | (~m & b & ~c) | (n & ~o & b & ~c) | (p & b & ~c) | d)", normalize(f1)->toString());
}


TEST_F(NormalizeTest, provyyy2)
{
    auto f1 = f("~(~((m & (n => o)) => p) | (b => c))");
    EXPECT_EQ("((~m & b & ~c) | (n & ~o & b & ~c) | (p & b & ~c))", normalize(f1)->toString());
}


TEST(Prover, prove333)
{
    //const std::string filename = "/Users/tbuenger/theorem/tptp/test/input/BOO109+1.p";
    //const std::string filename = "/Users/tbuenger/theorem/tptp/test/input/COM003+1.p";
    const std::string filename = "/Users/tbuenger/theorem/tptp/test/input/CSR031+1.p";

    auto problem = tptp::Problem::parseFromFile(filename);
    EXPECT_EQ(true, isTheorem(problem));
}






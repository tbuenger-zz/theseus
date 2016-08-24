#include <iostream>
#include <string>

#include "tptp/Problem.hpp"
#include "core/formula.hpp"
#include "core/transform.hpp"

using namespace tptp;
using namespace std;

int main()
{
    std::string input = "% -------------\n\
    % Status   : Theorem\n\
    % -------------\n\
    fof(ck,conjecture,(\n\
    ? [Z6,Z5,Z4,Z3,Z2,Z1,Z0] :\n\
      ( r(zero,zero,Z6)\n\
      & r(zero,Z6,Z5)\n\
      & r(zero,Z5,Z4)\n\
      & r(zero,Z4,Z3)\n\
      & r(zero,Z3,Z2)\n\
      & r(zero,Z2,Z1)\n\
      & r(zero,Z1,Z0) ) )).";

    Problem problem = Problem::parseFromString(input);

    for (auto& def : problem.definitions)
    {
        auto& f = def.formula;
        std::cout << *f << std::endl;


        {
            JunctionMerge jm(Operation::Conjunction);
            f->accept(jm);
        }
        {
            JunctionMerge jm(Operation::Disjunction);
            f->accept(jm);
        }

        std::cout << *f << std::endl;

    }



    std::cout << *parseFormula("![X,Y]:( p(f(X,g), g(Y,X,a,b)))") << std::endl;
}
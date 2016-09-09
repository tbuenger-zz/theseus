#pragma once

#include <memory>

class Formula;
namespace tptp
{
    class Problem;    
}


bool isTheorem(std::shared_ptr<Formula> f);
bool isTheorem(tptp::Problem& p);
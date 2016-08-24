#pragma once

#include <vector>
#include <memory>
#include <string>

class Formula;

namespace tptp {

class Definition
{
public:
    enum class Role
    {
        Axiom,
        Conjecture
    };

    Role role;
    std::string name;
    std::shared_ptr<Formula> formula;

    Definition() = default;
    Definition(Role role, std::string name, std::shared_ptr<Formula> formula) :
        role(role),
        name(std::move(name)),
        formula(std::move(formula))
    { }
};

}
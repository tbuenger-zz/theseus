#pragma once

#include <vector>
#include <memory>
#include <string>
#include "definition.hpp"

class Formula;

namespace tptp {

class Problem
{
public:
    enum class Status
    {
        Satisfiable,
        Theorem
    };

    std::string name;
    Status status;
    std::vector<Definition> definitions;

    Problem(std::string name, Status status) :
        name(std::move(name)),
        status(status),
        definitions()
    { }

    static Problem parseFromStream(std::istream& input);
    static Problem parseFromString(const std::string& input);
    static Problem parseFromFile(const std::string& filename);
};

std::shared_ptr<Formula> parseFormula(const std::string& input);

}
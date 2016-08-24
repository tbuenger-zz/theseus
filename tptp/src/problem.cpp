#include <fstream>
#include <sstream>
#include <regex>

#include "tptp/problem.hpp"
#include "tptp/scanner.hpp"

namespace tptp {

std::string extractMetaField(std::istream& iss, const std::string& field)
{
    std::regex status_regex("\\s*\\%\\s*"+field+"\\s*:\\s*(\\w+)");
    std::smatch match;
    for (std::string line; std::getline(iss, line); )
    {
        if (std::regex_match(line, match, status_regex))
        {
            if (match.size() == 2)
            {
                return match[1].str();
            }
        }
    }
    return "";
}

Problem::Status parseProblemStatus(const std::string& str)
{
    if (str == "Satisfiable")
        return Problem::Status::Satisfiable;
    if (str == "Theorem")
        return Problem::Status::Theorem;
    throw std::invalid_argument("received unknown Problem Status: " + str);
}


Problem Problem::parseFromStream(std::istream& in)
{
    std::string statusString = extractMetaField(in, "Status");
    Status status = parseProblemStatus(statusString);

    in.seekg(0);

    Scanner scanner(&in);
    scanner.setDebug(false);

    Problem problem("", status);

    Parser parser(scanner, problem);
    parser.set_debug_level(false);

    if (parser.parse() != 0)
    {
        throw std::invalid_argument("No proper TPTP Problem given.");
    }
    return problem;

}

Problem Problem::parseFromFile(const std::string &filename)
{
    std::ifstream in(filename.c_str());
    if (!in.good())
    {
        throw std::invalid_argument("No proper file.");
    }
    return parseFromStream(in);
}

Problem Problem::parseFromString(const std::string &input)
{
    std::istringstream iss(input);
    return parseFromStream(iss);
}

std::shared_ptr<Formula> parseFormula(const std::string &input)
{
    Problem p = Problem::parseFromString("% Status : Theorem\nfof(a,axiom," + input + ").");
    return p.definitions.front().formula;
}


}
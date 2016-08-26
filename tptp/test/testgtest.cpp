#include "gtest/gtest.h"
#include "tptp/problem.hpp"
#include "core/formula.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class StringParser : public ::testing::TestWithParam<const char*> { };

TEST_P(StringParser, ParseFormulaRoundTrip)
{
    const string expected = GetParam();
    const string given = tptp::parseFormula(expected)->toString();
    EXPECT_EQ(expected, given);

    const std::string filename = GetParam();
}

INSTANTIATE_TEST_CASE_P(tptp,  StringParser, ::testing::Values(
    "(p(X))",
    "(~ p(a,b))",
    "(p(f(a,b)))",
    "(p(a,b))",
    "(p(f(a,b)) & q(g(X),h,f(c,d)))",
    "(p(a) => p(b))",
    "(! [X] : p(X))",
    "(? [X,Y,Z] : p(X,Y,Z))",
    "(? [X] : p(X) => (q(a) | q(b) | q(f(c))))",
    "(? [X] : (p(X) => (q(a) & q(b) & q(f(c)))))"
    ));

vector<string> formulasFromFile(const string& filename)
{
    ifstream in(filename.c_str());
    const string fof = "fof";

    vector<string> result;
    ostringstream oss;
    bool recording = false;

    for (string line; getline(in, line); )
    {
        if (line.substr(0, 3) == fof)
        {
            size_t comma = line.find(',', line.find(',')+1);
            oss << line.substr(comma + 1);
            recording = true;
        }
        else if (recording)
        {
            size_t dot = line.find('.');
            if (dot != string::npos)
            {
                oss << line.substr(0, dot-1);
                    result.push_back(oss.str());
                oss = ostringstream();
                recording = false;
            }
            else
            {
                oss << line;
            }
        }
    }
    return result;
}

string normalizeFormula(string f)
{
    f.erase(remove(f.begin(), f.end(), ' '), f.end());
    f.erase(remove(f.begin(), f.end(), '\n'), f.end());
    f.erase(remove(f.begin(), f.end(), '\r'), f.end());
    f.erase(remove(f.begin(), f.end(), '\t'), f.end());
    return f;
}

class FileParser : public ::testing::TestWithParam<const char*> { };

TEST_P(FileParser, ParseProblems)
{
    const std::string filename = GetParam();
    auto definitions = tptp::Problem::parseFromFile(filename).definitions;
    auto expected = formulasFromFile(filename);

    EXPECT_EQ(expected.size(), definitions.size());

    mismatch(expected.begin(), expected.end(), definitions.begin(), [](const string& expected, const tptp::Definition& given){
        string normalizedExpected = normalizeFormula(expected);
        string normalizedGiven    = normalizeFormula(given.formula->toString());
        EXPECT_EQ(normalizedExpected, normalizedGiven);
        return normalizedExpected == normalizedGiven;
    });
}

INSTANTIATE_TEST_CASE_P(tptp, FileParser, ::testing::Values(
    "./input/COM003+1.p",
    "./input/BOO109+1.p",
    "./input/CSR031+1.p"
    ));
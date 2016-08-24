#include "gtest/gtest.h"
#include "tptp/problem.hpp"
#include "core/formula.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Tests factorial of 0.
TEST(FactorialTest, HandlesZeroInput) {
  EXPECT_EQ(1, 1);
}


TEST(TPTPParser, SimpleFormula) {
    string expected = "(p(a,b) & q(c))";
    const string result = tptp::parseFormula(expected)->toString();
    EXPECT_EQ(expected, result);
}

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
    /*if (f.front() != '(')
    {
        f = "(" +  f + ")";
    }*/
    return f;
}

class TPTPParser : public ::testing::TestWithParam<const char*> { };

TEST_P(TPTPParser, ParseFormulas)
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

INSTANTIATE_TEST_CASE_P(tptp,
                        TPTPParser,
                        ::testing::Values(
                            "/Users/tbuenger/theorem/tptp/test/COM003+1.p",
                            "/Users/tbuenger/theorem/tptp/test/BOO109+1.p",
                            "/Users/tbuenger/theorem/tptp/test/CSR031+1.p"
                            ));
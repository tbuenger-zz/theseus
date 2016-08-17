// $Id$
/** \file driver.cc Implementation of the example::Driver class. */

#include <fstream>
#include <sstream>

#include "driver.hpp"
#include "scanner.hpp"

namespace theseus {
namespace tptp {

Driver::Driver()
    : trace_scanning(false),
      trace_parsing(false),
      expressions()
{
}

bool Driver::parse_stream(std::istream& in, const std::string& sname)
{
    streamname = sname;

    Scanner scanner(&in);
    scanner.set_debug(trace_scanning);
    this->lexer = &scanner;

    Parser parser(*this);
    parser.set_debug_level(trace_parsing);
    return (parser.parse() == 0);
}

bool Driver::parse_file(const std::string &filename)
{
    std::ifstream in(filename.c_str());
    if (!in.good()) return false;
    return parse_stream(in, filename);
}

bool Driver::parse_string(const std::string &input, const std::string& sname)
{
    std::istringstream iss(input);
    return parse_stream(iss, sname);
}

void Driver::error(const class location& l,
           const std::string& m)
{
    std::cerr << l << ": " << m << std::endl;
}

void Driver::error(const std::string& m)
{
    std::cerr << m << std::endl;
}

} // namespace example
}


int main()
{
    using namespace theseus::tptp;
    Driver d;

    std::string problem = "fof(ck,conjecture,(\
    ? [Z6,Z5,Z4,Z3,Z2,Z1,Z0] :\
      ( r(zero,zero,Z6)\
      & r(zero,Z6,Z5)\
      & r(zero,Z5,Z4)\
      & r(zero,Z4,Z3)\
      & r(zero,Z3,Z2)\
      & r(zero,Z2,Z1)\
      & r(zero,Z1,Z0) ) )).";

    d.parse_string(problem);
    //d.parse_string("fof(grades_not_human,axiom,( ! [G] : ~ human(grade(G)) )). ");
    //d.parse_string("fof(grades_not_human,axiom, ~ human(grade(G))).");
    for (auto f : d.expressions)
    {
        std::cout << *f << std::endl;;
    }
}
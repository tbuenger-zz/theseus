#include "core/identifier.hpp"
#include <iostream>

using namespace std;

Identifier* IdentifierFactory::getOrAdd(Symbol symbol, bool isVariable)
{
    auto result = identifiers.emplace(symbol, nullptr);
    if (result.second)
    {
        result.first->second = make_unique<Identifier>(symbol, isVariable);
    }
    return result.first->second.get();
}

Symbol IdentifierFactory::unusedSymbol(Symbol prefix)
{
    Symbol symbol;
    do
    {
        symbol = prefix + unusedPrefix + to_string(counters[prefix]++);
    } while(identifiers.find(symbol) != identifiers.end());
    return symbol;
}

Identifier* IdentifierFactory::invent(Symbol symbol, bool isVariable)
{
    return getOrAdd(unusedSymbol(symbol), isVariable);
}


ostream& operator<< (ostream& stream, const Identifier& identifier)
{
    return stream << identifier.symbol;
}
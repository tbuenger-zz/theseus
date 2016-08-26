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

Symbol IdentifierFactory::unusedSymbol()
{
    Symbol symbol;
    do
    {
        symbol = unusedPrefix + to_string(counter++);
    } while(identifiers.find(symbol) != identifiers.end());
    return symbol;
}


ostream& operator<< (ostream& stream, const Identifier& identifier)
{
    return stream << identifier.symbol;
}
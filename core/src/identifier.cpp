#include "core/identifier.hpp"

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
        symbol = unusedPrefix + std::to_string(counter++);
    } while(identifiers.find(symbol) != identifiers.end());
    return symbol;
}
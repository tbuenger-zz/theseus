#include <string>
#include <iosfwd>
#include <unordered_map>
#include "core/utils.hpp"

typedef std::string Symbol;

class Identifier
{
public:
    Identifier(Symbol symbol, bool isVariable) :
        symbol(symbol),
        isVariable(isVariable)
    { }

    Symbol symbol;
    bool isVariable;
};


class IdentifierFactory : public Singleton<IdentifierFactory>
{
private:
    std::unordered_map<Symbol, std::unique_ptr<Identifier>> identifiers;
    size_t counter = 0;
    const std::string unusedPrefix = "_";

    Identifier* getOrAdd(Symbol symbol, bool isVariable);
    Symbol unusedSymbol();
    Identifier* invent() { return getOrAdd(unusedSymbol(), false); }

public:
    Identifier* variable(Symbol symbol) { return getOrAdd(symbol, true); }
    Identifier* function(Symbol symbol) { return getOrAdd(symbol, false); };
};


inline std::ostream& operator<< (std::ostream& stream, const Identifier& identifier)
{
    return stream << identifier.symbol;
}
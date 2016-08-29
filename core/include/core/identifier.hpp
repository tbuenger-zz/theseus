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
    std::unordered_map<Symbol, size_t> counters;
    const std::string unusedPrefix = "_";

    Identifier* getOrAdd(Symbol symbol, bool isVariable);
    Symbol unusedSymbol(Symbol prefix);
    Identifier* invent(Symbol symbol, bool isVariable);

public:
    Identifier* variable(Symbol symbol) { return getOrAdd(symbol, true); }
    Identifier* function(Symbol symbol) { return getOrAdd(symbol, false); };


    Identifier* inventFunction() { return invent("", false); }
    Identifier* inventFunction(Symbol symbol) { return invent(symbol, false); }

    Identifier* inventVariable() { return invent("", true); }
    Identifier* inventVariable(Symbol symbol) { return invent(symbol, true); }

    void clear() { identifiers.clear(); counters.clear(); }
};


std::ostream& operator<< (std::ostream& stream, const Identifier& identifier);
#include "cop/cop.hpp"
#include "cop/normalizer.hpp"
#include "core/literal.hpp"
#include "core/visitor.hpp"
#include "core/identifier.hpp"
#include "core/term.hpp"
#include "core/utils.hpp"
#include "tptp/problem.hpp"
#include "unify/unify.hpp"

#include "cop/persistent_list.hpp"


#include <algorithm>
#include <unordered_set>
#include <vector>
#include <iterator>
#include <iostream>
#include <sstream>

using namespace std;

namespace
{

class Clause
{
public:
    std::vector<Literal> literals;

    friend std::ostream& operator<< (std::ostream& stream, const Clause& clause);
};

std::ostream& operator<< (std::ostream& stream, const Clause& clause)
{
    return stream << "[" << join(clause.literals, ",") << "]";
}

class Matrix
{
public:
    std::vector<Clause> clauses;

    friend std::ostream& operator<< (std::ostream& stream, const Matrix& matrix);
};

std::ostream& operator<< (std::ostream& stream, const Matrix& matrix)
{
    return stream << "[" << join(matrix.clauses, ",\n") << "]";
}

class MatrixConverter : public FormulaVisitor
{
public:
    Matrix matrix;
    const Formula* root;

    MatrixConverter(const Formula* root) :
        matrix(),
        root(root)
    {
        matrix.clauses.emplace_back();
    }

    virtual void between(const Formula* op) override
    {
        if (op == root)
        {
            matrix.clauses.emplace_back();
        }
    }

    virtual void before(const FormulaLiteral* op) override
    {
        matrix.clauses.back().literals.emplace_back(op->literal);
    }
};

Matrix toMatrix(std::shared_ptr<Formula> f)
{

    f = normalize(f, false);
    cout << "non clausal: " << endl << *f << endl << endl;

    f = normalize(f);

    cout << *f << endl;

    MatrixConverter mc(f.get());
    f->accept(mc);
    return mc.matrix;
}


void listVariables(const Term* term, unordered_set<Symbol>& identifiers)
{
    if (term->identifier->isVariable)
    {
        identifiers.insert(term->identifier->symbol);
    }
    for (const Term* child : term->children)
    {
        listVariables(child, identifiers);
    }
}

Clause copyClause(const Clause& clause)
{
    unordered_set<Symbol> variables;
    for (auto& literal : clause.literals)
    {
        listVariables(literal.term, variables);
    }

    Clause copy = clause;

    for (auto& symbol : variables)
    {
        Identifier* variable = IdentifierFactory::instance().variable(symbol);
        Identifier* replacementVariable = IdentifierFactory::instance().inventVariable(symbol);
        Term* replacementTerm = TermDatabase::instance().get(replacementVariable);

        for (auto& literal : copy.literals)
        {
            literal.term = TermDatabase::instance().substitute(literal.term, variable, replacementTerm);
        }

    }

    return copy;
}

struct Goal
{
    Literal literal;
    PersistentList<Literal> path;

    Goal(Literal literal, PersistentList<Literal> path) :
        literal(move(literal)),
        path(move(path))
    { } 

    Goal(Literal literal) :
        literal(move(literal)),
        path()
    { } 
};


template <typename T>
ostream& operator<<(ostream& stream, const PersistentList<T>& list)
{
    stream << "[";
    list.forEach([&stream](const T& member){ stream << member << ", "; });
    stream << "]";
    return stream;
}


ostream& operator<<(ostream& stream, const Goal& goal)
{
    return stream << goal.literal;
}

struct State
{
    PersistentList<Goal> goals;
    Unifier unifier;
    PersistentList<Literal> lemmata;

    bool isTheorem(Matrix& m, size_t maxDepth, size_t nesting = 0)
    {
        cout << "[" << nesting << "] checking " << goals.size();
        if (goals.isEmpty())
        {
            cout << endl << " DONE!" << endl;
            return true;
        }
        cout << " goals " << goals << " with current goal <" << goals.front().literal << "> and path " << goals.front().path << endl;
        cout << "[" << nesting << "] lemmata: " << lemmata << endl; 

        const Goal& goal = goals.front();
        auto remainingGoals = goals.popped_front();

        if (goal.path.size() > maxDepth)
        {
            cout << "too long, going back" << endl;
            return false;
        }



        {
            // enforcing regularity
            bool done = false;
            goal.path.forEach([this, &done, &goal](const Literal& pathLiteral){

                if (!done
                 && pathLiteral.polarity == goal.literal.polarity
                 && pathLiteral.term->identifier == goal.literal.term->identifier)
                {
                    Unifier u = unifier;
                    try
                    {
                        u.unify(pathLiteral.term, goal.literal.term);
                        cout << "enforcing regularity by removing this subgoal" << endl;
                        done = true;

                    }
                    catch (NotUnifyableException)
                    { }
                }
            });

            if (done)
            {
                return false;
            }
        }

        {
            // check lemmatas
            bool done = false;
            lemmata.forEach([this, &done, &goal, &m, &remainingGoals, maxDepth, nesting](const Literal& lemma){

                if (!done
                 && lemma.polarity == goal.literal.polarity
                 && lemma.term->identifier == goal.literal.term->identifier)
                {
                    Unifier u = unifier;
                    try
                    {
                        u.unify(lemma.term, goal.literal.term);


                        State subState;
                        subState.goals = remainingGoals;
                        subState.unifier = u;
                        subState.lemmata = lemmata;

                        cout << "[" << nesting << "] reusing lemma " << lemma << endl;
                        //cin.get();
                        if (subState.isTheorem(m, maxDepth, nesting))
                        {
                            done = true;
                        }

                    }
                    catch (NotUnifyableException)
                    { }
                }
            });

            if (done)
            {
                return true;
            }
        }

        // check goal-path-conflict
        {
            bool done = false;

            auto innerLemmata = lemmata;
            goal.path.forEach([this, &innerLemmata, &done, &goal, &m, &remainingGoals, maxDepth, nesting](const Literal& pathLiteral){

                innerLemmata = innerLemmata.pushed_front(pathLiteral);

                if (!done
                 && pathLiteral.polarity != goal.literal.polarity
                 && pathLiteral.term->identifier == goal.literal.term->identifier)
                {
                    Unifier u = unifier;
                    try
                    {
                        u.unify(pathLiteral.term, goal.literal.term);


                        State subState;
                        subState.goals = remainingGoals;
                        subState.unifier = u;   

                        // add path up to conflict as lemmata
                        subState.lemmata = innerLemmata;

                        cout << "[" << nesting << "] short circuiting " << goal.literal << " with path member " << pathLiteral << endl;
                        //cin.get();
                        if (subState.isTheorem(m, maxDepth, nesting))
                        {
                            done = true;
                        }

                    }
                    catch (NotUnifyableException)
                    { }
                }
            });

            if (done)
            {
                return true;
            }
        }

        // extension step
        for (auto& clause : m.clauses)
        {
            Clause nextClause = copyClause(clause);
            for (auto& literal : nextClause.literals)
            {
                if (literal.polarity != goal.literal.polarity
                 && literal.term->identifier == goal.literal.term->identifier)
                {
                    Unifier u = unifier;
                    try
                    {
                        u.unify(literal.term, goal.literal.term);

                        State subState;
                        subState.goals = remainingGoals;

                        auto extendedPath = goal.path.pushed_front(goal.literal);

                        for (auto& otherLiteral : nextClause.literals) 
                        {
                            if (&literal == &otherLiteral)
                            {
                                continue;
                            }

                            Goal subGoal(otherLiteral, extendedPath);

                            subState.goals = subState.goals.pushed_front(subGoal);
                        }

                        subState.unifier = u;
                        subState.lemmata = lemmata;
                        // add full path if nextClause is empty.
                        if (nextClause.literals.size() == 1)
                        {
                            goal.path.forEach([&subState](const Literal& literal){
                                subState.lemmata = subState.lemmata.pushed_front(literal);
                            });
                        }
                        
                        cout << "[" << nesting << "] connecting " << goal.literal << " with " << literal << " from clause " << nextClause << endl;
                        //cin.get();
                        if (subState.isTheorem(m, maxDepth, nesting+1))
                        {
                            return true;
                        }

                    }
                    catch (NotUnifyableException)
                    { }
                }
            }
        }

        return false;
    }
};

class Prover
{
private:
    Matrix matrix;
public:
    Prover(std::shared_ptr<Formula> f) :
        matrix(toMatrix(f))
    { 
        std::reverse(matrix.clauses.begin(), matrix.clauses.end());
        cout << matrix << endl;
        //cin.get();
    }

    bool isTheorem()
    {
        for (size_t maxDepth = 10; ; ++maxDepth)
        //size_t maxDepth = 10;
        {
            cout << "maxDepth: " << maxDepth << endl;
            for (auto& clause : matrix.clauses)
            {
                State s;
                
                for (auto& literal : clause.literals)
                {
                    s.goals = s.goals.pushed_front(Goal(literal));
                }

                bool result = s.isTheorem(matrix, maxDepth);
                if (result)
                {
                    return true;
                }
            }
        }
        return false;
    }
};


}

bool isTheorem(std::shared_ptr<Formula> f)
{
    return Prover(f).isTheorem();
}

bool isTheorem(tptp::Problem& problem)
{
    vector<shared_ptr<Formula>> axioms;
    shared_ptr<Formula> conjecture;

    for (auto& def : problem.definitions)
    {
        if (def.role == tptp::Definition::Role::Axiom)
        {
            axioms.emplace_back(def.formula);
        }
        else
        {
            conjecture = def.formula;
        }
    }

    cout << "AXIOMS: " << axioms.size() << endl;
    cout << "Conjections: " << ((conjecture) ? 1 : 0) << endl; 

    return isTheorem(make_shared<FormulaImplication>(ImplicationDirection::Right, 
        make_shared<FormulaJunction>(Operation::Conjunction, move(axioms)),
        move(conjecture)));
}
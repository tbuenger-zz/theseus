#pragma once

#include "core/formula.hpp"
#include <unordered_set>


class JunctionMerge : public FormulaVisitor
{
private:
    Operation operation;
    std::unordered_set<const FormulaJunction*> candidates;
public:

    JunctionMerge(Operation operation) :
        operation(operation),
        candidates()
    { }

    virtual void before(const FormulaJunction * op) override
    {
        candidates.insert(op);
    }

    virtual std::shared_ptr<Formula> after(const FormulaJunction* op) override
    {
        std::vector<std::shared_ptr<Formula>> newChildren;
        for (auto& child : op->children)
        {
            auto found = candidates.find(reinterpret_cast<FormulaJunction*>(child.get()));
            if (found != candidates.end())
            {
                const FormulaJunction* candidate = *found;
                for (auto& grandchild : candidate->children)
                {
                    newChildren.emplace_back(std::move(grandchild));
                }
            }
            else
            {
                newChildren.emplace_back(std::move(child));
            }
        }
        return std::make_shared<FormulaJunction>(op->operation, std::move(newChildren));
    }


};

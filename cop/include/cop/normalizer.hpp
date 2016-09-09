#pragma once

#include <memory>

class Formula;

std::shared_ptr<Formula> normalize(std::shared_ptr<Formula> f, bool clausal=true);
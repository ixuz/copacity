#pragma once

#include "Constraint.hpp"

class SudokuConstraint : public Constraint {
public:
    bool propagate(
        Wfc& wfc,
        std::deque<std::size_t>& queue
    ) override;
};

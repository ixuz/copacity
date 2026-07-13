#pragma once

#include <deque>

class Wfc;

class Constraint {
public:
    virtual ~Constraint() = default;

    virtual bool propagate(
        Wfc& wfc,
        std::deque<std::size_t>& queue
    ) = 0;
};

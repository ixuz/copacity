#include "Wfc.hpp"
#include "Constraint.hpp"

namespace wfc {

std::vector<int> Wfc::solve() {
    if (!search()) {
        return {};
    }

    return snapshot();
}

bool Wfc::search() {

    if (!propagate()) {
        return false;
    }

    if (hasContradiction()) {
        return false;
    }

    const auto cell = lowestEntropyCell();

    if (cell == -1) {
        return true;
    }

    auto choices = possibleValues(cell);

    for (auto value : choices) {

        auto backup = copyDomains();

        collapse(cell, value);

        if (search()) {
            return true;
        }

        restore(backup);
    }

    return false;
}

bool Wfc::propagate() {

    std::deque<std::size_t> queue;

    for (std::size_t i = 0; i < length_; i++) {
        queue.push_back(i);
    }

    for (auto* constraint : constraints_) {
        if (!constraint->propagate(*this, queue)) {
            return false;
        }
    }

    return true;
}

int Wfc::lowestEntropyCell() const {

    int best = -1;
    auto bestCount = SIZE_MAX;

    for (std::size_t i = 0; i < length_; i++) {

        auto count = countPossible(i);

        if (count > 1 && count < bestCount) {
            best = static_cast<int>(i);
            bestCount = count;
        }
    }

    return best;
}

void Wfc::collapse(std::size_t cell, std::size_t value) {

    for (std::size_t i = 0; i < valueCount_; i++) {
        possible_[cell][i] = i == value;
    }
}

std::vector<int> Wfc::possibleValues(std::size_t cell) const {

    std::vector<int> result;

    for (std::size_t i = 0; i < valueCount_; i++) {
        if (possible_[cell][i]) {
            result.push_back(static_cast<int>(i));
        }
    }

    std::shuffle(
        result.begin(),
        result.end(),
        rng_
    );

    return result;
}

} // namespace wfc

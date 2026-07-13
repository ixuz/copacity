#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <random>
#include <vector>
#include <stdexcept>

class Constraint;

class Wfc {
public:
    Wfc(std::mt19937& rng, std::size_t length, std::size_t valueCount)
        : rng_(rng),
          length_(length),
          valueCount_(valueCount),
          possible_(length, std::vector<bool>(valueCount, true)) {}

    void addConstraint(Constraint* constraint) {
        constraints_.push_back(constraint);
    }

    void restrict(std::size_t cell, std::size_t value) {
        for (std::size_t i = 0; i < valueCount_; i++) {
            possible_[cell][i] = i == value;
        }
    }

    void forbid(std::size_t cell, std::size_t value) {
        possible_[cell][value] = false;
    }

    [[nodiscard]]
    std::vector<int> solve();

    [[nodiscard]]
    std::size_t countPossible(std::size_t cell) const {
        return std::count(
            possible_[cell].begin(),
            possible_[cell].end(),
            true
        );
    }

    [[nodiscard]]
    int singleValue(std::size_t cell) const {
        for (std::size_t i = 0; i < valueCount_; i++) {
            if (possible_[cell][i]) {
                return static_cast<int>(i);
            }
        }

        throw std::runtime_error("No value available");
    }

    bool removePossible(std::size_t cell, std::size_t value) {
        if (!possible_[cell][value]) {
            return false;
        }

        if (countPossible(cell) == 1) {
            return false;
        }

        possible_[cell][value] = false;
        return true;
    }

    [[nodiscard]]
    std::vector<int> snapshot() const {
        std::vector<int> result(length_, -1);

        for (std::size_t i = 0; i < length_; i++) {
            if (countPossible(i) == 1) {
                result[i] = singleValue(i);
            }
        }

        return result;
    }

private:
    bool search();

    bool propagate();

    bool hasContradiction() const {
        for (std::size_t i = 0; i < length_; i++) {
            if (countPossible(i) == 0) {
                return true;
            }
        }

        return false;
    }

    int lowestEntropyCell() const;

    void collapse(std::size_t cell, std::size_t value);

    std::vector<std::vector<bool>> copyDomains() const {
        return possible_;
    }

    void restore(const std::vector<std::vector<bool>>& state) {
        possible_ = state;
    }

    std::vector<int> possibleValues(std::size_t cell) const;

private:
    std::mt19937& rng_;
    std::size_t length_;
    std::size_t valueCount_;

    std::vector<std::vector<bool>> possible_;

    std::vector<Constraint*> constraints_;
};

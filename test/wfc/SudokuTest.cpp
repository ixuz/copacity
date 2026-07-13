#include <gtest/gtest.h>

#include "wfc/Wfc.hpp"
#include "wfc/Constraint.hpp"

class SudokuConstraint : public wfc::Constraint {
public:
    bool propagate(
        wfc::Wfc& wfc,
        std::deque<std::size_t>& queue
    ) override {
        while (!queue.empty()) {

            const auto cell = queue.front();
            queue.pop_front();

            if (wfc.countPossible(cell) != 1) {
                continue;
            }

            const auto value = wfc.singleValue(cell);

            const int row = static_cast<int>(cell / 9);
            const int col = static_cast<int>(cell % 9);

            auto remove = [&](std::size_t other) {
                if (other == cell) {
                    return true;
                }

                if (wfc.removePossible(other, value)) {
                    if (wfc.countPossible(other) == 0) {
                        return false;
                    }

                    queue.push_back(other);
                }

                return true;
            };

            // Row
            for (int c = 0; c < 9; c++) {
                if (!remove(row * 9 + c)) {
                    return false;
                }
            }

            // Column
            for (int r = 0; r < 9; r++) {
                if (!remove(r * 9 + col)) {
                    return false;
                }
            }

            // Box
            const int boxRow = (row / 3) * 3;
            const int boxCol = (col / 3) * 3;

            for (int r = boxRow; r < boxRow + 3; r++) {
                for (int c = boxCol; c < boxCol + 3; c++) {
                    if (!remove(r * 9 + c)) {
                        return false;
                    }
                }
            }
        }

        return true;
    };
};

TEST(WfcSudokuTest, SolvesSudokuPuzzle)
{
    // Arrange
    int puzzle[9][9] = {
        {0,0,6,0,1,0,0,4,0},
        {0,0,0,4,0,0,8,0,0},
        {0,0,0,2,0,0,6,9,5},
        {0,1,0,0,8,0,0,0,4},
        {0,6,0,0,0,0,0,1,0},
        {9,0,0,0,3,0,0,5,0},
        {1,2,8,0,0,5,0,0,0},
        {0,0,5,0,0,8,0,0,0},
        {0,4,0,0,2,0,5,0,0}
    };

    int expected[81] = {
        3,5,6,8,1,9,7,4,2,
        2,9,7,4,5,6,8,3,1,
        4,8,1,2,7,3,6,9,5,
        5,1,3,7,8,2,9,6,4,
        8,6,2,5,9,4,3,1,7,
        9,7,4,6,3,1,2,5,8,
        1,2,8,3,6,5,4,7,9,
        7,3,5,9,4,8,1,2,6,
        6,4,9,1,2,7,5,8,3
    };

    std::mt19937 rng{123};

    wfc::Wfc wfc(rng, 81, 9);

    SudokuConstraint sudoku;
    wfc.addConstraint(&sudoku);

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {

            const int value = puzzle[row][col];

            if (value != 0) {
                // Internally values are zero-based.
                wfc.restrict(
                    row * 9 + col,
                    value - 1
                );
            }
        }
    }

    // Act
    auto result = wfc.solve();

    // Assert
    ASSERT_FALSE(result.empty());

    ASSERT_EQ(result.size(), 81);

    for (std::size_t i = 0; i < result.size(); i++) {
        EXPECT_EQ(result[i] + 1, expected[i]);
    }
}

#include <gtest/gtest.h>

#include "wfc/Wfc.hpp"
#include "wfc/SudokuConstraint.hpp"

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

    Wfc wfc(rng, 81, 9);

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

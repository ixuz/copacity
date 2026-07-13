#include "SudokuConstraint.hpp"
#include "Wfc.hpp"

bool SudokuConstraint::propagate(
    Wfc& wfc,
    std::deque<std::size_t>& queue
) {
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
}

#include "sudoku_grid.hpp"

#include <iostream>
#include <string>

void print_grid(const Grid& board)
{
    for (const auto& row : board) {
        for (int value : row) {
            std::cout << value << ' ';
        }
    }
}

int main(int argc, char* argv[])
{
    Grid initial_board{{
        {{0, 0, 0, 0, 0, 7, 0, 9, 0}},
        {{0, 3, 0, 0, 2, 0, 0, 0, 8}},
        {{0, 0, 9, 6, 0, 0, 5, 0, 0}},
        {{0, 0, 5, 3, 0, 0, 9, 0, 0}},
        {{0, 1, 0, 0, 8, 0, 0, 0, 2}},
        {{6, 0, 0, 0, 0, 4, 0, 0, 0}},
        {{3, 0, 0, 0, 0, 0, 0, 1, 0}},
        {{0, 4, 0, 0, 0, 0, 0, 0, 7}},
        {{0, 0, 7, 0, 0, 0, 3, 0, 0}}
    }};

    // Send the hardcoded board to Python.
    if (argc > 1 && std::string(argv[1]) == "--initial") {
        print_grid(initial_board);
        return 0;
    }

    // Otherwise, read a board from Python and solve it.
    Grid board{};

    for (std::size_t row = 0; row < ROWS; ++row) {
        for (std::size_t col = 0; col < COLUMNS; ++col) {
            if (!(std::cin >> board[row][col])) {
                return 1;
            }
        }
    }

    try {
        SudokuGrid sudoku(board);
		sudoku.solve(Algo::BACKTRACK);
        print_grid(sudoku.board());
    }
    catch (const std::exception&) {
        return 1;
    }

    return 0;
}
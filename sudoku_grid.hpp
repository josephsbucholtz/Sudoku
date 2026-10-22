#include <cstdio>

#include <array>
#include <set>
#include <stdexcept>
#include <utility>

#define ROWS 9
#define COLUMNS 9

using Grid = std::array<std::array<int, 9>, 9>;
using Cell = std::pair<size_t, size_t>;


class SudokuGrid {
public:
	//
	//Need to check if initState is valid!
	//
	explicit SudokuGrid(Grid initState) :
		grid_(initState)
	{
		if (!isValidBoard(initState))
		{
			throw std::runtime_error("Invalid Sudoku Board.");
		}
		if (!backtrack(initState))
		{
			throw std::runtime_error("Unsovlable Sudoku Board");
		}

		//initialize starting positions
		for (size_t row {}; row < ROWS; row++)
		{
			for (size_t col {}; col < COLUMNS; col++)
			{
				if (grid_[row][col] != 0) 
				{
					initPositions_[row][col] = true;
				}
				else 
				{
					initPositions_[row][col] = false;
				}
			}
		}
	}

	//No copy 
	SudokuGrid(const SudokuGrid&) = delete;
	SudokuGrid& operator=(const SudokuGrid&) = delete;

	//Move semantics 
	SudokuGrid(SudokuGrid&& other) noexcept :
		grid_(std::move(other.grid_)),
		initPositions_(std::move(other.initPositions_))
	{}

	SudokuGrid& operator=(SudokuGrid&& other) noexcept
	{
		if (this != &other)
		{
			grid_ = std::move(other.grid_);
			initPositions_ = std::move(other.initPositions_);
		}

		return *this;
	}


	/*
	 * Not checking if it is also consistent.
	 * For the moment, it works because initial board is valid and 
	 * we solve with backtrack. If we want users to solve, then
	 * we have to check consistency as well.
	 */
	[[nodiscard]]
	bool is_complete(const Grid& board) const
	{
		for (size_t row {}; row < ROWS; row++)
		{
			for (size_t col {}; col < COLUMNS; col++)
			{
				if (board[row][col] == 0) 
				{
					return false;
				}
			}
		}

		if (!isValidBoard(board))
		{
			return false;
		}

		return true;
	}

	[[nodiscard]]
	bool is_consistent(Cell cell, int value, const Grid& board) const
	{

		// check row
		for (size_t col {}; col < COLUMNS; col++)
		{
			if (col != cell.second && board[cell.first][col] == value)
			{
				return false;
			}
		}

		// check column
		for (size_t row {}; row < ROWS; row++)
		{
			if (row != cell.first && board[row][cell.second] == value)
			{
				return false;
			}
		}

		//check 3x3 grid
		int start_row {(static_cast<int>(cell.first) / 3) * 3};
		int start_col {(static_cast<int>(cell.second) / 3) * 3};

		for (int tg_row { start_row }; tg_row < start_row + 3; tg_row++ )
		{
			for (int tg_col { start_col }; tg_col < start_col + 3; tg_col++ )
			{
				if ((tg_row != static_cast<int>(cell.first) ||
     				tg_col != static_cast<int>(cell.second)) &&
    				board[tg_row][tg_col] == value)
				{
    				return false;
				}
			}
		}

		return true;
	}

	Cell select_unassigned_variable(const Grid& board)
	{
		for (size_t row {}; row < ROWS; row++)
		{
			for (size_t col {}; col < COLUMNS; col++)
			{
				if (board[row][col] == 0) 
				{
					return Cell(row, col);
				}
			}
		}

		return Cell();
	}

	bool backtrack(Grid& board)
	{
		if (is_complete(board))
		{
			return true;
		}

		Cell current_cell { select_unassigned_variable(board) };

		for (auto value : ordered_domain_values)
		{
			if (is_consistent(current_cell, value, board))
			{
				board[current_cell.first][current_cell.second] = value;
				if (backtrack(board))
				{
					return true;
				}
				board[current_cell.first][current_cell.second] = 0;
			}
		}
		return false;
	}

	bool backtracking_AC3(Grid board)
	{
		if (is_complete(board))
		{
			return true;
		}

		return false;
	}

	void printBoard()
	{
		for (size_t row {}; row < ROWS; row++)
		{
			for (size_t col {}; col < COLUMNS; col++)
			{
				printf("%d ", grid_[row][col]);
			}
			printf("\n");
		}
	}

	void reset() 
	{
		for (size_t row {}; row < ROWS; row++)
		{
			for (size_t col {}; col < COLUMNS; col++)
			{
				if (!initPositions_[row][col])
				{
					grid_[row][col] = 0;
				}
			}
		}
	}

	bool isValidBoard(const Grid& board) const
	{
		for (size_t row {}; row < ROWS; row++)
		{
			for (size_t col {}; col < COLUMNS; col++)
			{
				if (board[row][col] != 0)
				{
					if (!is_consistent(Cell(row, col), board[row][col], board))
					{
						return false;
					}
				}
			}
		}

		return true;
	}

	void solve()
	{
		backtrack(grid_);
	}

private:
	Grid grid_ {};
	std::array<std::array<bool, 9>, 9> initPositions_ {};
	std::set<int> ordered_domain_values {1, 2, 3, 4, 5, 6, 7, 8, 9};
};

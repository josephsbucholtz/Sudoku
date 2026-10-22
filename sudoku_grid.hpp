#include <cstdio>

#include <set>
#include <utility>
#include <vector>

using Grid = std::vector<std::vector<int>>;
using Cell = std::pair<int, int>;


class SudokuGrid {
public:
	explicit SudokuGrid(Grid initState) : 
	grid_(initState)
	{

	}

	//No copy 
	SudokuGrid(const SudokuGrid&) = delete;
	SudokuGrid& operator=(const SudokuGrid&) = delete;

	//Move semantics 
	SudokuGrid(SudokuGrid&& other) :
		grid_(other.grid_) 
	{}

	SudokuGrid& operator=(SudokuGrid&& other)
	{
		if (this != &other)
		{
			grid_ = other.grid_;
		}

		return *this;
	}


	/*
	 * Not checking if it is also consistent.
	 */
	[[nodiscard]]
	bool is_complete()
	{
		for (size_t row {}; row < 9; row++)
		{
			for (size_t col {}; col < 9; col++)
			{
				if (grid_[row][col] == 0) 
				{
					return false;
				}
			}
		}

		return true;
	}

	[[nodiscard]]
	bool is_consistent(Cell cell, int value)  
	{

		// check row
		for (size_t col {}; col < 9; col++)
		{
			if (grid_[cell.first][col] == value)
			{
				return false;
			}
		}

		// check column
		for (size_t row {}; row < 9; row++)
		{
			if (grid_[row][cell.second] == value)
			{
				return false;
			}
		}

		//check 3x3 grid
		int start_row {(cell.first / 3) * 3};
		int start_col {(cell.second / 3) * 3};
		for (int tg_row { start_row }; tg_row < start_row + 3; tg_row++ )
		{
			for (int tg_col { start_col }; tg_col < start_col + 3; tg_col++ )
			{
				if (grid_[tg_row][tg_col] == value)
				{
					return false;
				}
			}
		}

		return true;
	}

	Cell select_unassigned_variable()
	{
		for (size_t row {}; row < 9; row++)
		{
			for (size_t col {}; col < 9; col++)
			{
				if (grid_[row][col] == 0) 
				{
					return Cell(row, col);
				}
			}
		}

		return Cell();
	}

	bool backtrack()
	{
		if (is_complete())
		{
			return true;
		}

		Cell current_cell { select_unassigned_variable() };

		for (auto value : ordered_domain_values)
		{
			if (is_consistent(current_cell, value))
			{
				grid_[current_cell.first][current_cell.second] = value;
				if (backtrack())
				{
					return true;
				}
				grid_[current_cell.first][current_cell.second] = 0;
			}
		}
		return false;
	}

	void printBoard()
	{
		for (size_t row {}; row < 9; row++)
		{
			for (size_t col {}; col < 9; col++)
			{
				printf("%d ", grid_[row][col]);
			}
			printf("\n");
		}
	}


private:
	Grid grid_ {};
	std::set<int> ordered_domain_values {1, 2, 3, 4, 5, 6, 7, 8, 9};
};

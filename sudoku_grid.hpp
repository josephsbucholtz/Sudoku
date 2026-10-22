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




private:
	Grid grid_ {};
};

#include <vector>

using Grid = std::vector<std::vector<int>>;

class SudokuGrid {
public:
	explicit SudokuGrid(Grid initState) : 
	grid_(initState)
	{

	}

	bool is_complete()
	{
		for (size_t row {}; row < grid_.size(); row++)
		{
			for (size_t col {}; col < grid_.size(); col++)
			{
				if (grid_[row][col] == 0) 
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

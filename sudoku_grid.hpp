#include <cstdio>

#include <array>
#include <queue>
#include <set>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <vector>

#define ROWS 9
#define COLUMNS 9

using Grid = std::array<std::array<int, 9>, 9>;
using Cell = std::pair<size_t, size_t>;
using Domain = std::array<std::array<std::unordered_set<int>, 9>, 9>;

enum class Algo {
	BACKTRACK,
	AC3
};


class SudokuGrid {
public:
	//
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

	/*
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

	Cell select_unassigned_variable_MRV(const Grid& board)
	{
		Cell smalleset_domain {};
		int minimum_length { 10 };
		for (size_t row {}; row < ROWS; row++)
		{
			for (size_t col {}; col < COLUMNS; col++)
			{
				if (board[row][col] == 0)
				{
					if (grid_domains_[row][col].size() < minimum_length)
					{
						minimum_length = grid_domains_[row][col].size();
						smalleset_domain = {row, col};

						if (minimum_length == 1)
						{
							return smalleset_domain;
						}
					}
				}

			}
		}

		return smalleset_domain;
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

	void solve(Algo algo)
	{
		switch (algo) {
			case Algo::BACKTRACK:
				backtrack(grid_);
				break;
			case Algo::AC3:
				domains_for_grid();
				backtracking_AC3(grid_);
				break;
			default:
				break;
		}
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

	void domains_for_grid()
	{
		for (size_t row {}; row < ROWS; row++)
		{
			for (size_t col {}; col < COLUMNS; col++)
			{
				if (grid_[row][col] > 0)
				{
					grid_domains_[row][col] = {grid_[row][col]};
				}
				else if (grid_[row][col] == 0)
				{
					grid_domains_[row][col] = ordered_domain_values;
				}
			}
		}

	}

	std::set<Cell> neighbors(const Cell& cell)
	{
		std::set<Cell> neighbors_set {};

		for (size_t k {}; k < 9; k++)
		{
			if (k != cell.first)
			{
				neighbors_set.insert({k, cell.second});
			}
			if (k != cell.second)
			{
				neighbors_set.insert({cell.first, k});
			}
		}

		// ADD 3x3 neighbors
		int start_row {(static_cast<int>(cell.first) / 3) * 3};
		int start_col {(static_cast<int>(cell.second) / 3) * 3};

		for (int tg_row { start_row }; tg_row < start_row + 3; tg_row++ )
		{
			for (int tg_col { start_col }; tg_col < start_col + 3; tg_col++ )
			{
				if (tg_row != static_cast<int>(cell.first) || tg_col != static_cast<int>(cell.second))
				{
					neighbors_set.insert(Cell({static_cast<size_t>(tg_row), static_cast<size_t>(tg_col)}));
				}
			}
		}

		return neighbors_set;
	}

	bool revise(const Cell& cell_1, const Cell& cell_2)
	{
		std::unordered_set<int>& i_domain = grid_domains_[cell_1.first][cell_1.second];
		const std::unordered_set<int>& j_domain = grid_domains_[cell_2.first][cell_2.second];

		if (j_domain.size() == 1)
		{
			int value { *j_domain.begin() };
			if (i_domain.find(value) != i_domain.end())
			{
				i_domain.erase(value);
				return true;
			}
		}

		return false;
	}

	bool ac_3()
	{
		std::queue<std::pair<Cell, Cell>> queue {};
		for (size_t row {}; row < ROWS; row++)
		{
			for (size_t col {}; col < COLUMNS; col++) 
			{
				Cell Xi {row, col};
				std::set<Cell> cell_neighbors = neighbors(Xi);
				for (const Cell& cell : cell_neighbors)
				{
					queue.push({Xi, cell});
				}
			}
		}

		while (!queue.empty())
		{
			auto curr_cell_pair { queue.front() };
			queue.pop();
			

			if (revise(curr_cell_pair.first, curr_cell_pair.second))
			{
				if (grid_domains_[curr_cell_pair.first.first][curr_cell_pair.first.second].empty())
				{
					return false;
				}

				for (const Cell& neighbor : neighbors(curr_cell_pair.first))
				{
					if (neighbor != curr_cell_pair.second)
					{
						queue.push({neighbor, curr_cell_pair.first});
					}
				}
			}
		}

		return true;
	}

	bool backtracking_AC3(Grid& board)
	{
		if (is_complete(board))
		{
			return true;
		}

		Cell current_cell { select_unassigned_variable_MRV(board) };

		std::unordered_set<int> canidates = grid_domains_[current_cell.first][current_cell.second];


		for (int value : canidates)
		{
			if (is_consistent(current_cell, value, board))
			{
				Domain saved_domains = grid_domains_;
				board[current_cell.first][current_cell.second] = value;
				grid_domains_[current_cell.first][current_cell.second] = {value};

				if (ac_3())
				{
					std::vector<Cell> forced {};

					for (size_t row {}; row < ROWS; row++)
					{
						for (size_t col {}; col < COLUMNS; col++) 
						{
							if (board[row][col] == 0 && grid_domains_[row][col].size() == 1)
							{
								auto it { grid_domains_[row][col].begin() };
								board[row][col] = *it;
								forced.push_back({row, col});
							}
						}
					}

					if (backtracking_AC3(board))
					{
						return true;
					}

					for (Cell cell : forced)
					{
						board[cell.first][cell.second] = 0;
					}
				}
				board[current_cell.first][current_cell.second] = 0;
				grid_domains_ = std::move(saved_domains);
			}
		}

		return false;
	}


private:
	Grid grid_ {};
	Domain grid_domains_ {};
	std::array<std::array<bool, 9>, 9> initPositions_ {};
	std::unordered_set<int> ordered_domain_values {1, 2, 3, 4, 5, 6, 7, 8, 9};
};

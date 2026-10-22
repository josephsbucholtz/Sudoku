#include <cassert>
#include <cstdio>

#include "sudoku_grid.hpp"

int main() 
{
	Grid initState = {{
		{0,9,0,0,0,7,5,2,0},
    	{0,0,6,8,0,0,0,0,0},
    	{0,0,0,3,0,9,1,0,8},
    	{2,8,0,7,0,0,0,0,0},
    	{6,0,7,0,0,0,3,8,0},
    	{0,0,0,0,0,5,0,0,2},
    	{0,0,3,5,1,0,0,0,0},
    	{8,1,0,9,7,0,4,5,3},
    	{9,6,5,4,2,0,8,0,7} }};

	Grid initial_hard = {{
    	{0,0,0,0,0,7,0,9,0},
    	{0,3,0,0,2,0,0,0,8},
    	{0,0,9,6,0,0,5,0,0},
    	{0,0,5,3,0,0,9,0,0},
    	{0,1,0,0,8,0,0,0,2},
    	{6,0,0,0,0,4,0,0,0},
    	{3,0,0,0,0,0,0,1,0},
    	{0,4,0,0,0,0,0,0,7},
    	{0,0,7,0,0,0,3,0,0}
	}};

	SudokuGrid sg(initState);
	
	printf("INTIAL BOARD ---- \n");
	sg.printBoard();

	printf("\n");

	sg.solve();
	printf("\n");

	printf("SOLVED BOARD ---- \n");
	sg.printBoard();

	printf("\n");

	printf("RESET BOARD ---- \n");
	sg.reset();
	sg.printBoard();


	return 0;
}

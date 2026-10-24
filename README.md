# Sudoku

A Sudoku solver implemented in C++ using:

- Backtracking
- Arc Consistency (AC-3)


# Results

BACKTRACKING: 4691μs     # of nodes: 11588

AC3: 15201μs             # of nodes: 15

-My results show Backtracking to perform ~3x faster, but traverse 11,500 nodes.
Conversely, AC-3 searches 15 nodes, but is ~3x slower. I believe the overhead cost
in AC-3 is contributing to the slow down. Fixes could give a more accurate description.

## Installation

```bash
git clone <repo>
cd Sudoku
```

# Run Instructions
```bash
g++ sudoku_grid.hpp main.cpp -o build
./build
```

# Future Considerations

My results show a ~3x slower performance for AC-3 but that may not true.
There seems to be a lot of overhead cost for each search node and potential
optimizations could give a more accurate picture. Here is a list of improvments:

1. Use 9-bit mask instead of unordered_set 
2. Precompute the neighbors for each entry
3. over excess copies of domain.


#include "Clock.hpp"
#include <iostream>
#include <vector>

const bool prev_and_alive_neighbors_to_next[2][9] = {
    // dead cell
    {false, false, false, true, false, false, false, false, false},
    // alive cell
    {false, false, true, true, false, false, false, false, false}};
// Change function signature to take C_PADDED
inline bool advance_single(const bool *prev, const unsigned int C_PADDED, int r, int c)
{
  // C_PADDED is C + 2
  unsigned int living_neighbors = 0;

  // Store C_PADDED to avoid repetitive indexing calculations in the inner loop
  const int stride = C_PADDED;

  // Pointer to the start of the 3x3 neighborhood's top-left
  const bool idx = (r - 1) * stride + (c - 1);

  // Unroll or simplify the neighbor loop
  living_neighbors += prev[idx];     // (r-1, c-1)
  living_neighbors += prev[idx + 1]; // (r-1, c)
  living_neighbors += prev[idx + 2]; // (r-1, c+1)

  living_neighbors += prev[stride + 0]; // (r, c-1)
  // living_neighbors += prev[stride + 1]; // (r, c) - Skip center!
  living_neighbors += prev[stride + 2]; // (r, c+1)

  living_neighbors += prev[2 * stride + 0]; // (r+1, c-1)
  living_neighbors += prev[2 * stride + 1]; // (r+1, c)
  living_neighbors += prev[2 * stride + 2]; // (r+1, c+1)

  // Current cell is base_ptr[stride + 1]
  bool is_this_cell_alive = prev[stride + 1];

  // The previous logic counted the center cell and subtracted it:
  // living_neighbors -= is_this_cell_alive;
  // Our simplified neighbor sum above already excluded the center cell.

  return prev_and_alive_neighbors_to_next[is_this_cell_alive][living_neighbors];
}

void advance(bool *cur, bool *prev, const unsigned int R, const unsigned int C)
{
  const unsigned int C_PADDED = C + 2;
  // Iterate over the R x C inner grid
  for (unsigned int i = 1; i < R + 1; ++i)
    for (unsigned int j = 1; j < C + 1; ++j)
      // Pass C_PADDED instead of R and C
      cur[i * C_PADDED + j] = advance_single(prev, C_PADDED, i, j);
}

void print_board(const bool *board, const unsigned int R, const unsigned int C)
{
  for (unsigned int i = 1; i < R + 1; ++i)
  {
    for (unsigned int j = 1; j < C + 1; ++j)
      std::cout << int(board[i * (C + 2) + j]);
    std::cout << std::endl;
  }
}

int main(int argc, char **argv)
{
  if (argc == 4)
  {
    const unsigned int R = 1 << 10;
    const unsigned int C = 1 << 11;
    unsigned long random_seed = atoi(argv[1]);
    unsigned int NUM_REPS = atoi(argv[2]);
    bool print = bool(atoi(argv[3]));

    srand(random_seed);
    bool *prev = (bool *)calloc((R + 2) * (C + 2), sizeof(bool));
    bool *cur = (bool *)calloc((R + 2) * (C + 2), sizeof(bool));

    for (unsigned int i = 1; i < R + 1; ++i)
      for (unsigned int j = 1; j < C + 1; ++j)
        cur[i * (C + 2) + j] = (rand() % 2 == 0);

    if (print)
      print_board(cur, R, C);

    Clock c;

    for (unsigned int rep = 0; rep < NUM_REPS; ++rep)
    {
      std::swap(cur, prev);
      advance(cur, prev, R, C);
    }

    c.ptock();

    if (print)
      print_board(cur, R, C);
  }
  else
  {
    std::cerr << "usage game-of-life <generations> <0:don't print, 1:print>" << std::endl;
  }

  return 0;
}
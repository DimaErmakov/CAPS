#include "Clock.hpp"
#include <iostream>

// Read about constexpr in chapter 10
constexpr bool next_state_table[2][9] = {
    {false, false, false, true, false, false, false, false, false},
    {false, false, true, true, false, false, false, false, false}};

// Read about inline in chapter 10
__attribute__((always_inline)) inline void compute_all_cumulative_sums(const bool *__restrict previous, unsigned int *__restrict cumulative_sums, unsigned int R, unsigned int C_PADDING)
{
  // Double layer padding is needed, since for the top left cell, you need to access (i - 2) * C_PADDING + (j - 2)
  for (unsigned int i = 2; i < R + 4; ++i) {
    unsigned int row_sum = 0;
    for (unsigned int j = 2; j < C_PADDING; ++j) {
      row_sum += previous[i * C_PADDING + j];
      cumulative_sums[i * C_PADDING + j] = row_sum + cumulative_sums[(i - 1) * C_PADDING + j];
    }
  }
}

__attribute__((always_inline)) inline unsigned int compute_three_by_three_grid(const unsigned int *__restrict cumulative_sums, unsigned int C_PADDING, unsigned int i, unsigned int j)
{
  // This calculation makes a lot of sense when you draw a picture
  return cumulative_sums[(i + 1) * C_PADDING + (j + 1)] + cumulative_sums[(i - 2) * C_PADDING + (j - 2)] - (cumulative_sums[(i - 2) * C_PADDING + (j + 1)] + cumulative_sums[(i + 1) * C_PADDING + (j - 2)]);
}

// Read about restrict in chapter 11
void advance(bool *__restrict current, const bool *__restrict previous, unsigned int R, unsigned int C, unsigned int *__restrict cumulative_sums)
{
  const unsigned int C_PADDING = C + 4;

  compute_all_cumulative_sums(previous, cumulative_sums, R, C_PADDING);

  for (unsigned int i = 2; i < R + 2; ++i) {
    for (unsigned int j = 2; j < C + 2; ++j) {
      bool current_cell = previous[i * C_PADDING + j];
      current[i * C_PADDING + j] = next_state_table[current_cell][compute_three_by_three_grid(cumulative_sums, C_PADDING, i, j) - current_cell];
    }
  }
}

void print_board(const bool *board, unsigned int R, unsigned int C)
{
  for (unsigned int i = 2; i < R + 2; ++i) {
    for (unsigned int j = 2; j < C + 2; ++j)
      std::cout << int(board[i * (C + 4) + j]);
    std::cout << std::endl;
  }
}

int main(int argc, char **argv)
{
  if (argc != 4) {
    std::cerr << "usage: game-of-life <seed> <generations> <0/1 print>\n";
    return 1;
  }

  const unsigned int R = 1 << 10;
  const unsigned int C = 1 << 11;

  unsigned long random_seed = atoi(argv[1]);
  unsigned int NUM_REPS = atoi(argv[2]);
  bool print = bool(atoi(argv[3]));

  srand(random_seed);

  bool *current = (bool *)calloc((R + 4) * (C + 4), sizeof(bool));
  bool *previous = (bool *)calloc((R + 4) * (C + 4), sizeof(bool));
  unsigned int *cumulative_sums = (unsigned int *)calloc((R + 4) * (C + 4), sizeof(unsigned int));

  for (unsigned int i = 2; i < R + 2; ++i)
    for (unsigned int j = 2; j < C + 2; ++j)
      current[i * (C + 4) + j] = (rand() % 2 == 0);

  if (print)
    print_board(current, R, C);

  Clock c;
  for (unsigned int rep = 0; rep < NUM_REPS; ++rep)
  {
    std::swap(current, previous);
    advance(current, previous, R, C, cumulative_sums);
  }
  c.ptock();

  if (print)
    print_board(current, R, C);

  free(current);
  free(previous);
  free(cumulative_sums);
  return 0;
}

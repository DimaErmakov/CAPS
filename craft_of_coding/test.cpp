#include <iostream>
#include <iomanip>
#include <chrono>

class Clock
{
protected:
  std::chrono::high_resolution_clock::time_point start_time;

public:
  Clock()
  {
    tick();
  }
  void tick()
  {
    start_time = std::chrono::high_resolution_clock::now();
  }
  auto tock()
  {
    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
  }
  // Print elapsed time with newline
  void ptock()
  {
    auto elapsed = tock();
    std::cout << "Took " << elapsed << "ns ~ " << double(elapsed) / 1e9 << "s" << std::endl;
  }
};
#include <math.h>

// NOTE: FOR YOUR OWN SAFETY, DO NOT EDIT THIS CODE:
__attribute__((noinline)) void naive_mult(double *__restrict C, double *__restrict B, double *__restrict A, const unsigned int N)
{
  for (unsigned int i = 0; i < N; ++i)
    for (unsigned int j = 0; j < N; ++j)
    {
      C[i * N + j] = 0.0;
      for (unsigned int k = 0; k < N; ++k)
        C[i * N + j] += A[i * N + k] * B[k * N + j];
    }
}
// END NOTE

constexpr unsigned int LEAF_SIZE = 64;

__attribute__((always_inline)) inline void add(double *__restrict C, const double *__restrict A, const double *__restrict B, unsigned int N)
{
  for (unsigned int i = 0; i < N * N; ++i)
    C[i] = A[i] + B[i];
}

__attribute__((always_inline)) inline void subtract(double *__restrict C, const double *__restrict A, const double *__restrict B, unsigned int N)
{
  for (unsigned int i = 0; i < N * N; ++i)
    C[i] = A[i] - B[i];
}
__attribute__((always_inline)) inline void transpose(double *__restrict B_T,
                                                     const double *__restrict B,
                                                     unsigned int N)
{
  for (unsigned int i = 0; i < N; ++i)
    for (unsigned int j = 0; j < N; ++j)
      B_T[j * N + i] = B[i * N + j];
}

__attribute__((always_inline)) inline void multiply(double *__restrict C, const double *__restrict A, const double *__restrict B, unsigned int N)

{
  double *B_T = new double[N * N];

  transpose(B_T, B, N);

  for (unsigned int i = 0; i < N * N; ++i)
    C[i] = 0.0;

  for (unsigned int i = 0; i < N; ++i)
    for (unsigned int k = 0; k < N; ++k)
    {
      double a = A[i * N + k];
      for (unsigned int j = 0; j < N; ++j)
        C[i * N + j] += a * B_T[j * N + k]; // B is now row wise
    }
  delete[] B_T;
}
// unsigned long get_total_temp_memory_needed_for_all_recursions(unsigned int N)
// {
//   if (N <= LEAF_SIZE)
//   {
//     return 0;
//   }
//   unsigned int new_size = N >> 1;
//   unsigned int sub_matrix_size = new_size * new_size;
//   // There will be 17 temporary matrices for each recursion except for the base case
//   return (17 * sub_matrix_size) + get_total_temp_memory_needed_for_all_recursions(new_size);
// }
__attribute__((always_inline)) inline unsigned long get_total_temp_memory_needed_for_all_recursions(unsigned int N)
{
  unsigned long total = 0;

  while (true)
  {
    unsigned int new_size = N >> 1;
    total += 17UL * new_size * new_size;
    N = new_size;
    if (N <= LEAF_SIZE)
      return total;
  }
}

void strassen_recursive(double *__restrict C, const double *__restrict A, const double *__restrict B, double *__restrict workspace, unsigned int N)
{
  if (N <= LEAF_SIZE)
  {
    multiply(C, A, B, N);
    return;
  }

  unsigned int new_size = N >> 1;
  unsigned int sub_matrix_size = new_size * new_size;

  double *A11 = workspace;
  double *A12 = A11 + sub_matrix_size;
  double *A21 = A12 + sub_matrix_size;
  double *A22 = A21 + sub_matrix_size;
  double *B11 = A22 + sub_matrix_size;
  double *B12 = B11 + sub_matrix_size;
  double *B21 = B12 + sub_matrix_size;
  double *B22 = B21 + sub_matrix_size;
  double *M1 = B22 + sub_matrix_size;
  double *M2 = M1 + sub_matrix_size;
  double *M3 = M2 + sub_matrix_size;
  double *M4 = M3 + sub_matrix_size;
  double *M5 = M4 + sub_matrix_size;
  double *M6 = M5 + sub_matrix_size;
  double *M7 = M6 + sub_matrix_size;
  double *T1 = M7 + sub_matrix_size;
  double *T2 = T1 + sub_matrix_size;
  double *next_workspace = T2 + sub_matrix_size;

  for (unsigned int i = 0; i < new_size; ++i)
    for (unsigned int j = 0; j < new_size; ++j)
    {
      unsigned int sub_matrix_index_one = i * N + j;
      unsigned int sub_matrix_index_two = i * N + (j + new_size);
      unsigned int sub_matrix_index_three = (i + new_size) * N + j;
      unsigned int sub_matrix_index_four = (i + new_size) * N + (j + new_size);

      unsigned int partition_index = i * new_size + j;

      A11[partition_index] = A[sub_matrix_index_one];
      A12[partition_index] = A[sub_matrix_index_two];
      A21[partition_index] = A[sub_matrix_index_three];
      A22[partition_index] = A[sub_matrix_index_four];

      B11[partition_index] = B[sub_matrix_index_one];
      B12[partition_index] = B[sub_matrix_index_two];
      B21[partition_index] = B[sub_matrix_index_three];
      B22[partition_index] = B[sub_matrix_index_four];
    }

  // M1 = (A11 + A22) * (B11 + B22)
  add(T1, A11, A22, new_size);
  add(T2, B11, B22, new_size);
  strassen_recursive(M1, T1, T2, next_workspace, new_size);

  // M2 = (A21 + A22) * B11
  add(T1, A21, A22, new_size);
  strassen_recursive(M2, T1, B11, next_workspace, new_size);

  // M3 = A11 * (B12 - B22)
  subtract(T1, B12, B22, new_size);
  strassen_recursive(M3, A11, T1, next_workspace, new_size);

  // M4 = A22 * (B21 - B11)
  subtract(T1, B21, B11, new_size);
  strassen_recursive(M4, A22, T1, next_workspace, new_size);

  // M5 = (A11 + A12) * B22
  add(T1, A11, A12, new_size);
  strassen_recursive(M5, T1, B22, next_workspace, new_size);

  // M6 = (A21 - A11) * (B11 + B12)
  subtract(T1, A21, A11, new_size);
  add(T2, B11, B12, new_size);
  strassen_recursive(M6, T1, T2, next_workspace, new_size);

  // M7 = (A12 - A22) * (B21 + B22)
  subtract(T1, A12, A22, new_size);
  add(T2, B21, B22, new_size);
  strassen_recursive(M7, T1, T2, next_workspace, new_size);

  for (unsigned int i = 0; i < new_size; ++i)
    for (unsigned int j = 0; j < new_size; ++j)
    {
      unsigned int partition_index = i * new_size + j;
      double C11 = M1[partition_index] + M4[partition_index] - M5[partition_index] + M7[partition_index];
      double C12 = M3[partition_index] + M5[partition_index];
      double C21 = M2[partition_index] + M4[partition_index];
      double C22 = M1[partition_index] - M2[partition_index] + M3[partition_index] + M6[partition_index];

      C[i * N + j] = C11;
      C[i * N + (j + new_size)] = C12;
      C[(i + new_size) * N + j] = C21;
      C[(i + new_size) * N + (j + new_size)] = C22;
    }
}

__attribute__((noinline)) void mult(double *__restrict C, double *__restrict B, double *__restrict A, const unsigned int N)
{

  unsigned long total_temp_memory_size = get_total_temp_memory_needed_for_all_recursions(N);

  double *workspace = new double[total_temp_memory_size];

  strassen_recursive(C, A, B, workspace, N);

  delete[] workspace;
}

// NOTE: FOR YOUR OWN SAFETY, DO NOT EDIT THIS CODE:
int main(int argc, char **argv)
{
  // if (argc != 3)
  //   std::cerr << "usage: <log_problem_size> <seed>" << std::endl;
  // else
  // {
  const unsigned LOG_N = 10;
  const unsigned long N = 1ul << LOG_N;
  const unsigned seed = 42;
  srand(seed);
  double *A = new double[N * N];
  double *B = new double[N * N];
  for (unsigned long i = 0; i < N * N; ++i)
    A[i] = (rand() % 1000) / 999.0;
  for (unsigned long i = 0; i < N * N; ++i)
    B[i] = (rand() % 1000) / 999.0;
  double *C = new double[N * N];
  Clock c;
  mult(C, B, A, N);
  double mult_time = c.tock();
  c.ptock();
  double *C_naive = new double[N * N];
  c.tick();
  naive_mult(C_naive, B, A, N);
  double naive_mult_time = c.tock();
  double max_l1_error = 0.0;
  for (unsigned long i = 0; i < N * N; ++i)
    max_l1_error = std::max(max_l1_error, fabs(C_naive[i] - C[i]));
  std::cout << "Numeric error: " << max_l1_error << std::endl;
  double speedup = naive_mult_time / mult_time;
  std::cout << "Speedup: " << speedup << std::endl;
  std::cout << std::endl;
  // Verify numeric error:
  bool pass = true;
  if (max_l1_error >= 1e-4)
  {
    std::cerr << "FAIL: Numeric error is too high" << std::endl;
    pass = false;
  }
  else
    std::cout << "PASS: Numeric error" << std::endl;
  // Verify speedup:
  if (speedup < 5)
  {
    std::cerr << "FAIL: Speedup is too low" << std::endl;
    pass = false;
  }
  else
    std::cout << "PASS: Speedup" << std::endl;
  if (pass)
    std::cout << "OVERALL PASS" << std::endl;
  else
  {
    std::cout << "OVERALL FAIL" << std::endl;
    exit(1);
  }
  // }
  return 0;
}
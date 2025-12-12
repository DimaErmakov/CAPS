#include "Clock.hpp"
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
__attribute__((always_inline)) inline void multiply(double *__restrict C, const double *__restrict A, const double *__restrict B, unsigned int N)
{
  for (unsigned int i = 0; i < N * N; ++i)
    C[i] = 0.0;

  for (unsigned int i = 0; i < N; ++i)
    for (unsigned int k = 0; k < N; ++k)
    {
      const double a = A[i * N + k];
      for (unsigned int j = 0; j < N; ++j)
        C[i * N + j] += a * B[k * N + j];
    }
}
__attribute__((always_inline)) inline unsigned int get_total_temp_memory_needed_for_all_recursions(unsigned int N, unsigned int amount_of_temp_matrices)
{
  unsigned int total = 0;

  while (true)
  {
    const unsigned int new_size = N >> 1;
    total += amount_of_temp_matrices * new_size * new_size;
    N = new_size;
    if (N <= LEAF_SIZE)
      return total;
  }
}

void strassen_recursive(double *__restrict C, const double *__restrict A, const double *__restrict B, unsigned int N, double *__restrict combined_workspace)
{
  if (N <= LEAF_SIZE)
  {
    multiply(C, A, B, N);
    return;
  }

  const unsigned int new_size = N >> 1;
  const unsigned int sub_matrix_size = new_size * new_size;

  double *__restrict A11 = new double[sub_matrix_size];
  double *__restrict A12 = new double[sub_matrix_size];
  double *__restrict A21 = new double[sub_matrix_size];
  double *__restrict A22 = new double[sub_matrix_size];
  double *__restrict B11 = new double[sub_matrix_size];
  double *__restrict B12 = new double[sub_matrix_size];
  double *__restrict B21 = new double[sub_matrix_size];
  double *__restrict B22 = new double[sub_matrix_size];

  double *__restrict M1 = combined_workspace;
  double *__restrict M2 = M1 + sub_matrix_size;
  double *__restrict M3 = M2 + sub_matrix_size;
  double *__restrict M4 = M3 + sub_matrix_size;
  double *__restrict M5 = M4 + sub_matrix_size;
  double *__restrict M6 = M5 + sub_matrix_size;
  double *__restrict M7 = M6 + sub_matrix_size;

  double *__restrict T1 = M7 + sub_matrix_size;
  double *__restrict T2 = T1 + sub_matrix_size;
  double *__restrict T3 = T2 + sub_matrix_size;
  double *__restrict T4 = T3 + sub_matrix_size;
  double *__restrict T5 = T4 + sub_matrix_size;
  double *__restrict T6 = T5 + sub_matrix_size;
  double *__restrict T7 = T6 + sub_matrix_size;
  double *__restrict T8 = T7 + sub_matrix_size;
  double *__restrict T9 = T8 + sub_matrix_size;
  double *__restrict T10 = T9 + sub_matrix_size;
  double *__restrict next_combined_workspace = T10 + sub_matrix_size;

  for (unsigned int i = 0; i < new_size; ++i)
    for (unsigned int j = 0; j < new_size; ++j)
    {
      const unsigned int sub_matrix_index_one = i * N + j;
      const unsigned int sub_matrix_index_two = i * N + (j + new_size);
      const unsigned int sub_matrix_index_three = (i + new_size) * N + j;
      const unsigned int sub_matrix_index_four = (i + new_size) * N + (j + new_size);

      const unsigned int index = i * new_size + j;

      A11[index] = A[sub_matrix_index_one];
      A12[index] = A[sub_matrix_index_two];
      A21[index] = A[sub_matrix_index_three];
      A22[index] = A[sub_matrix_index_four];

      B11[index] = B[sub_matrix_index_one];
      B12[index] = B[sub_matrix_index_two];
      B21[index] = B[sub_matrix_index_three];
      B22[index] = B[sub_matrix_index_four];
    }

#pragma omp task
  add(S1, A11, A22, new_size);

#pragma omp task
  add(S2, B11, B22, new_size);

#pragma omp task
  add(S3, A21, A22, new_size);

#pragma omp task
  subtract(S4, B12, B22, new_size);

#pragma omp task
  subtract(S5, B21, B11, new_size);

#pragma omp task
  add(S6, A11, A12, new_size);

#pragma omp task
  subtract(S7, A21, A11, new_size);

#pragma omp task
  add(S8, B11, B12, new_size);

#pragma omp task
  subtract(S9, A12, A22, new_size);

#pragma omp task
  add(S10, B21, B22, new_size);

#pragma omp taskwait

#pragma omp task
  strassen_recursive(M1, T1, T2, new_size, next_combined_workspace);

#pragma omp task
  strassen_recursive(M2, T3, B11, new_size, next_combined_workspace);

#pragma omp task
  strassen_recursive(M3, A11, T4, new_size, next_combined_workspace);

#pragma omp task
  strassen_recursive(M4, A22, T5, new_size, next_combined_workspace);

#pragma omp task
  strassen_recursive(M5, T6, B22, new_size, next_combined_workspace);

#pragma omp task
  strassen_recursive(M6, T7, T8, new_size, next_combined_workspace);

#pragma omp task
  strassen_recursive(M7, T9, T10, new_size, next_combined_workspace);
#pragma omp taskwait
  for (unsigned int i = 0; i < new_size; ++i)
    for (unsigned int j = 0; j < new_size; ++j)
    {
      const unsigned int index = i * new_size + j;

      C[i * N + j] = M1[index] + M4[index] - M5[index] + M7[index];
      C[i * N + (j + new_size)] = M3[index] + M5[index];
      ;
      C[(i + new_size) * N + j] = M2[index] + M4[index];
      C[(i + new_size) * N + (j + new_size)] = M1[index] - M2[index] + M3[index] + M6[index];
    }
  delete[] A11;
  delete[] A12;
  delete[] A21;
  delete[] A22;
  delete[] B11;
  delete[] B12;
  delete[] B21;
  delete[] B22;
}

__attribute__((noinline)) void mult(double *__restrict C, double *__restrict B, double *__restrict A, const unsigned int N)
{
  const unsigned int total_T_and_M = get_total_temp_memory_needed_for_all_recursions(N, 17);
  // const unsigned int total_M = get_total_temp_memory_needed_for_all_recursions(N, 7);

  double *combined_workspace = new double[total_T_and_M];

  // double *S_workspace = combined_workspace;
  // double *M_workspace = combined_workspace + total_T;

  // #pragma omp parallel
  // #pragma omp single
  strassen_recursive(C, A, B, N, combined_workspace);

  delete[] combined_workspace;
}

// NOTE: FOR YOUR OWN SAFETY, DO NOT EDIT THIS CODE:
int main(int argc, char **argv)
{
  if (argc != 3)
    std::cerr << "usage: <log_problem_size> <seed>" << std::endl;
  else
  {
    const unsigned LOG_N = atoi(argv[1]);
    const unsigned long N = 1ul << LOG_N;
    const unsigned seed = atoi(argv[2]);
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
  }
  return 0;
}
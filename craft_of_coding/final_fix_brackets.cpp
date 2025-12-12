
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

constexpr unsigned int LEAF_SIZE = 64ul;

__attribute__((always_inline)) inline void add(double *__restrict C, const double *__restrict A, const double *__restrict B, unsigned int N)
{
#pragma omp simd
  for (unsigned int i = 0; i < N * N; ++i)
    C[i] = A[i] + B[i];
}

__attribute__((always_inline)) inline void subtract(double *__restrict C, const double *__restrict A, const double *__restrict B, unsigned int N)
{
#pragma omp simd
  for (unsigned int i = 0; i < N * N; ++i)
    C[i] = A[i] - B[i];
}
__attribute__((always_inline)) inline void multiply(double *__restrict C, const double *__restrict A, const double *__restrict B, unsigned int N)
{
#pragma omp parallel for
  for (unsigned int i = 0; i < N * N; ++i)
    C[i] = 0.0;

#pragma omp parallel for
  for (unsigned int i = 0; i < N; ++i)
    for (unsigned int k = 0; k < N; ++k)
    {
      double a = A[i * N + k];
#pragma omp simd
      for (unsigned int j = 0; j < N; ++j)
        C[i * N + j] += a * B[k * N + j];
    }
}
__attribute__((always_inline)) inline unsigned long get_total_temp_memory_needed_for_all_recursions(unsigned int N)
{
  unsigned long total = 0;

  while (true)
  {
    unsigned int new_matrix_size = N >> 1;
    // There will be 17 temporary matrices for each recursion
    total += 17ul * new_matrix_size * new_matrix_size;
    N = new_matrix_size;
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

  unsigned int new_matrix_size = N >> 1;
  unsigned int sub_matrix_size = new_matrix_size * new_matrix_size;

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

  for (unsigned int i = 0; i < new_matrix_size; ++i)
    for (unsigned int j = 0; j < new_matrix_size; ++j)
    {
      unsigned int matrix_11_index = i * N + j;
      unsigned int matrix_12_index = i * N + (j + new_matrix_size);
      unsigned int matrix_21_index = (i + new_matrix_size) * N + j;
      unsigned int matrix_22_index = (i + new_matrix_size) * N + (j + new_matrix_size);

      unsigned int index = i * new_matrix_size + j;

      A11[index] = A[matrix_11_index];
      A12[index] = A[matrix_12_index];
      A21[index] = A[matrix_21_index];
      A22[index] = A[matrix_22_index];

      B11[index] = B[matrix_11_index];
      B12[index] = B[matrix_12_index];
      B21[index] = B[matrix_21_index];
      B22[index] = B[matrix_22_index];
    }

  // M1 = (A11 + A22) * (B11 + B22)
  add(T1, A11, A22, new_matrix_size);
  add(T2, B11, B22, new_matrix_size);
#pragma omp task
  strassen_recursive(M1, T1, T2, next_workspace, new_matrix_size);

  // M2 = (A21 + A22) * B11
  add(T1, A21, A22, new_matrix_size);
#pragma omp task
  strassen_recursive(M2, T1, B11, next_workspace, new_matrix_size);

  // M3 = A11 * (B12 - B22)
  subtract(T1, B12, B22, new_matrix_size);
#pragma omp task
  strassen_recursive(M3, A11, T1, next_workspace, new_matrix_size);

  // M4 = A22 * (B21 - B11)
  subtract(T1, B21, B11, new_matrix_size);
#pragma omp task
  strassen_recursive(M4, A22, T1, next_workspace, new_matrix_size);

  // M5 = (A11 + A12) * B22
  add(T1, A11, A12, new_matrix_size);
#pragma omp task
  strassen_recursive(M5, T1, B22, next_workspace, new_matrix_size);

  // M6 = (A21 - A11) * (B11 + B12)
  subtract(T1, A21, A11, new_matrix_size);
  add(T2, B11, B12, new_matrix_size);
#pragma omp task
  strassen_recursive(M6, T1, T2, next_workspace, new_matrix_size);

  // M7 = (A12 - A22) * (B21 + B22)
  subtract(T1, A12, A22, new_matrix_size);
  add(T2, B21, B22, new_matrix_size);
#pragma omp task
  strassen_recursive(M7, T1, T2, next_workspace, new_matrix_size);

#pragma omp taskwait
  for (unsigned int i = 0; i < new_matrix_size; ++i)
    for (unsigned int j = 0; j < new_matrix_size; ++j)
    {
      unsigned int index = i * new_matrix_size + j;
      double C11 = M1[index] + M4[index] - M5[index] + M7[index];
      double C12 = M3[index] + M5[index];
      double C21 = M2[index] + M4[index];
      double C22 = M1[index] - M2[index] + M3[index] + M6[index];

      C[i * N + j] = C11;
      C[i * N + (j + new_matrix_size)] = C12;
      C[(i + new_matrix_size) * N + j] = C21;
      C[(i + new_matrix_size) * N + (j + new_matrix_size)] = C22;
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
    double max_l1_rel_error = 0.0;
    for (unsigned long i = 0; i < N * N; ++i)
      // NOTE: unstable when C[i] is 0.
      max_l1_rel_error = std::max(max_l1_rel_error, fabs(C_naive[i] - C[i]) /
                                                        C[i]);
    std::cout << "Numeric error: " << max_l1_rel_error << std::endl;
    double speedup = naive_mult_time / mult_time;
    std::cout << "Speedup: " << speedup << std::endl;
    std::cout << std::endl;
    // Verify numeric error:
    bool pass = true;
    if (max_l1_rel_error >= 1e-4)
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
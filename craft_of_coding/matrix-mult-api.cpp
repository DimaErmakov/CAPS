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
void add(double *C, const double *A, const double *B, unsigned int N)
{
  for (unsigned int i = 0; i < N * N; ++i)
    C[i] = A[i] + B[i];
}

void subtract(double *C, const double *A, const double *B, unsigned int N)
{
  for (unsigned int i = 0; i < N * N; ++i)
    C[i] = A[i] - B[i];
}

void multiply(double *C, const double *A, const double *B, unsigned int N)
{
  for (unsigned int i = 0; i < N; ++i)
    for (unsigned int j = 0; j < N; ++j)
    {
      C[i * N + j] = 0.0;
      for (unsigned int k = 0; k < N; ++k)
        C[i * N + j] += A[i * N + k] * B[k * N + j];
    }
}
// Recursive Strassen function (with workstation)
void strassen(double *C, const double *A, const double *B, double *workstation, unsigned int N, unsigned int leaf_size)
{
  if (N <= leaf_size)
  {
    multiply(C, A, B, N);
    return;
  }

  unsigned int newSize = N / 2;
  unsigned int subMatrixSize = newSize * newSize;

  // Assign workspace regions
  double *M1 = workstation;
  double *M2 = M1 + subMatrixSize;
  double *M3 = M2 + subMatrixSize;
  double *M4 = M3 + subMatrixSize;
  double *M5 = M4 + subMatrixSize;
  double *M6 = M5 + subMatrixSize;
  double *M7 = M6 + subMatrixSize;
  double *T1 = M7 + subMatrixSize;
  double *T2 = T1 + subMatrixSize;
  double *next_ws = T2 + subMatrixSize; // Workspace for recursive calls

  // Compute base addresses for A and B submatrices (no copying)
  const double *A11 = A;
  const double *A12 = A + newSize;
  const double *A21 = A + newSize * N;
  const double *A22 = A + newSize * N + newSize;

  const double *B11 = B;
  const double *B12 = B + newSize;
  const double *B21 = B + newSize * N;
  const double *B22 = B + newSize * N + newSize;

  // M1 = (A11 + A22) * (B11 + B22)
  add(T1, A11, A22, newSize);
  add(T2, B11, B22, newSize);
  strassen(M1, T1, T2, next_ws, newSize, leaf_size);

  // M2 = (A21 + A22) * B11
  add(T1, A21, A22, newSize);
  strassen(M2, T1, B11, next_ws, newSize, leaf_size);

  // M3 = A11 * (B12 - B22)
  subtract(T1, B12, B22, newSize);
  strassen(M3, A11, T1, next_ws, newSize, leaf_size);

  // M4 = A22 * (B21 - B11)
  subtract(T1, B21, B11, newSize);
  strassen(M4, A22, T1, next_ws, newSize, leaf_size);

  // M5 = (A11 + A12) * B22
  add(T1, A11, A12, newSize);
  strassen(M5, T1, B22, next_ws, newSize, leaf_size);

  // M6 = (A21 - A11) * (B11 + B12)
  subtract(T1, A21, A11, newSize);
  add(T2, B11, B12, newSize);
  strassen(M6, T1, T2, next_ws, newSize, leaf_size);

  // M7 = (A12 - A22) * (B21 + B22)
  subtract(T1, A12, A22, newSize);
  add(T2, B21, B22, newSize);
  strassen(M7, T1, T2, next_ws, newSize, leaf_size);

  // Combine results into C
  for (unsigned int i = 0; i < newSize; ++i)
  {
    for (unsigned int j = 0; j < newSize; ++j)
    {
      unsigned int idx = i * newSize + j;
      double C11 = M1[idx] + M4[idx] - M5[idx] + M7[idx];
      double C12 = M3[idx] + M5[idx];
      double C21 = M2[idx] + M4[idx];
      double C22 = M1[idx] - M2[idx] + M3[idx] + M6[idx];

      C[i * N + j] = C11;
      C[i * N + (j + newSize)] = C12;
      C[(i + newSize) * N + j] = C21;
      C[(i + newSize) * N + (j + newSize)] = C22;
    }
  }
}

__attribute__((noinline)) void mult(double *__restrict C, double *__restrict B, double *__restrict A, const unsigned int N)
{
  const unsigned int leaf_size = 64;
  const unsigned int subMatrixCount = 9; // M1..M7 + T1 + T2
  double *workstation = new double[subMatrixCount * (N * N) / 4];
  strassen(C, A, B, workstation, N, leaf_size);
  delete[] workstation;
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

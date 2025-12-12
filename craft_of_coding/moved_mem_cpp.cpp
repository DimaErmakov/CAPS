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
__attribute__((always_inline)) inline void add(double *C, const double *A, const double *B, unsigned int N)
{
  for (unsigned int i = 0; i < N * N; ++i)
    C[i] = A[i] + B[i];
}

__attribute__((always_inline)) inline void subtract(double *C, const double *A, const double *B, unsigned int N)
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

// Recursive Strassen function
void strassen(double *C, const double *A, const double *B, unsigned int N, unsigned int leaf_size,
              double *A11, double *A12, double *A21, double *A22,
              double *B11, double *B12, double *B21, double *B22,
              double *M1, double *M2, double *M3, double *M4, double *M5, double *M6, double *M7,
              double *T1, double *T2)
{
  if (N <= leaf_size)
  {
    multiply(C, A, B, N);
    return;
  }
  unsigned int newSize = N / 2;

  // Divide matrices into submatrices
  for (unsigned int i = 0; i < newSize; ++i)
    for (unsigned int j = 0; j < newSize; ++j)
    {
      unsigned int i1 = i * N + j;
      unsigned int i2 = i * N + (j + newSize);
      unsigned int i3 = (i + newSize) * N + j;
      unsigned int i4 = (i + newSize) * N + (j + newSize);

      unsigned int index = i * newSize + j;

      A11[index] = A[i1];
      A12[index] = A[i2];
      A21[index] = A[i3];
      A22[index] = A[i4];

      B11[index] = B[i1];
      B12[index] = B[i2];
      B21[index] = B[i3];
      B22[index] = B[i4];
    }

  // M1 = (A11 + A22) * (B11 + B22)
  add(T1, A11, A22, newSize);
  add(T2, B11, B22, newSize);
  strassen(M1, T1, T2, newSize, leaf_size, A11, A12, A21, A22, B11, B12, B21, B22, M1, M2, M3, M4, M5, M6, M7, T1, T2);

  // M2 = (A21 + A22) * B11
  add(T1, A21, A22, newSize);
  strassen(M2, T1, B11, newSize, leaf_size, A11, A12, A21, A22, B11, B12, B21, B22, M1, M2, M3, M4, M5, M6, M7, T1, T2);

  // M3 = A11 * (B12 - B22)
  subtract(T1, B12, B22, newSize);
  strassen(M3, A11, T1, newSize, leaf_size, A11, A12, A21, A22, B11, B12, B21, B22, M1, M2, M3, M4, M5, M6, M7, T1, T2);

  // M4 = A22 * (B21 - B11)
  subtract(T1, B21, B11, newSize);
  strassen(M4, A22, T1, newSize, leaf_size, A11, A12, A21, A22, B11, B12, B21, B22, M1, M2, M3, M4, M5, M6, M7, T1, T2);

  // M5 = (A11 + A12) * B22
  add(T1, A11, A12, newSize);
  strassen(M5, T1, B22, newSize, leaf_size, A11, A12, A21, A22, B11, B12, B21, B22, M1, M2, M3, M4, M5, M6, M7, T1, T2);

  // M6 = (A21 - A11) * (B11 + B12)
  subtract(T1, A21, A11, newSize);
  add(T2, B11, B12, newSize);
  strassen(M6, T1, T2, newSize, leaf_size, A11, A12, A21, A22, B11, B12, B21, B22, M1, M2, M3, M4, M5, M6, M7, T1, T2);

  // M7 = (A12 - A22) * (B21 + B22)
  subtract(T1, A12, A22, newSize);
  add(T2, B21, B22, newSize);
  strassen(M7, T1, T2, newSize, leaf_size, A11, A12, A21, A22, B11, B12, B21, B22, M1, M2, M3, M4, M5, M6, M7, T1, T2);

  // Combine results
  for (unsigned int i = 0; i < newSize; ++i)
    for (unsigned int j = 0; j < newSize; ++j)
    {
      unsigned int index = i * newSize + j;
      double C11 = M1[index] + M4[index] - M5[index] + M7[index];
      double C12 = M3[index] + M5[index];
      double C21 = M2[index] + M4[index];
      double C22 = M1[index] - M2[index] + M3[index] + M6[index];

      C[i * N + j] = C11;
      C[i * N + (j + newSize)] = C12;
      C[(i + newSize) * N + j] = C21;
      C[(i + newSize) * N + (j + newSize)] = C22;
    }
}

__attribute__((noinline)) void mult(double *__restrict C, double *__restrict B, double *__restrict A, const unsigned int N)
{

  unsigned int newSize = N / 2;
  unsigned int subMatrixSize = newSize * newSize;
  // Allocate temporary matrices
  double *A11 = new double[subMatrixSize];
  double *A12 = new double[subMatrixSize];
  double *A21 = new double[subMatrixSize];
  double *A22 = new double[subMatrixSize];
  double *B11 = new double[subMatrixSize];
  double *B12 = new double[subMatrixSize];
  double *B21 = new double[subMatrixSize];
  double *B22 = new double[subMatrixSize];
  double *M1 = new double[subMatrixSize];
  double *M2 = new double[subMatrixSize];
  double *M3 = new double[subMatrixSize];
  double *M4 = new double[subMatrixSize];
  double *M5 = new double[subMatrixSize];
  double *M6 = new double[subMatrixSize];
  double *M7 = new double[subMatrixSize];
  double *T1 = new double[subMatrixSize];
  double *T2 = new double[subMatrixSize];
  strassen(C, A, B, N, 32, A11, A12, A21, A22,
           B11, B12, B21, B22,
           M1, M2, M3, M4, M5, M6, M7,
           T1, T2);
  delete[] A11;
  delete[] A12;
  delete[] A21;
  delete[] A22;
  delete[] B11;
  delete[] B12;
  delete[] B21;
  delete[] B22;
  delete[] M1;
  delete[] M2;
  delete[] M3;
  delete[] M4;
  delete[] M5;
  delete[] M6;
  delete[] M7;
  delete[] T1;
  delete[] T2;
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
#include "Clock.hpp"
#include <random>
#include <iostream>
#include <stdexcept>

void selection_sort(double *source, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
  {
    unsigned long min_ind = i;
    double min_val = source[i];
    for (unsigned long j = i + 1; j < n; ++j)
      if (source[j] < min_val)
      {
        min_ind = j;
        min_val = source[j];
      }

    std::swap(source[i], source[min_ind]);
  }
}

void merge_sort(double *buffer, double *source, const unsigned long n)
{
  if (n <= 32)
  {
    selection_sort(source, n);
    return;
  }

  double *source_2 = source + n / 2;
  merge_sort(buffer, source, n / 2);
  merge_sort(buffer + n / 2, source_2, n - n / 2);

  // Merge sorted halves into buffer:
  unsigned long i = 0, j = 0, buffer_ind = 0;
  while (i < n / 2 && j < (n - n / 2))
  {
    if (source[i] < source_2[j])
    {
      buffer[buffer_ind] = source[i];
      ++i;
    }
    else
    {
      // In case of equality, order doesn't matter, so use this case:
      buffer[buffer_ind] = source_2[j];
      ++j;
    }

    ++buffer_ind;
  }
  // Copy remaining values:
  for (; i < n / 2; ++i, ++buffer_ind)
    buffer[buffer_ind] = source[i];
  for (; j < n - n / 2; ++j, ++buffer_ind)
    buffer[buffer_ind] = source_2[j];

  // Copy back sorted list from buffer:
  for (i = 0; i < n; ++i)
    source[i] = buffer[i];
}

void my_sort(double *x, unsigned long n);

void time_my_sort_and_compare_to_merge_sort(const double *x, unsigned long n)
{
  Clock c;

  std::cerr << "\nTIMING MERGE SORT vs. MY SORT..." << std::endl;

  // run merge sort and time:
  std::cout << "merge sort:\n";
  double *x_merge_sort = new double[n];
  std::copy(x, x + n, x_merge_sort);
  double *buffer = new double[n];
  c.tick();
  merge_sort(buffer, x_merge_sort, n);
  auto time_merge_sort = c.tock();
  c.ptock();
  delete[] buffer;

  // call my_sort and time:
  std::cout << "my sort:\n";
  double *x_my_sort = new double[n];
  std::copy(x, x + n, x_my_sort);
  c.tick();
  my_sort(x_my_sort, n);
  auto time_my_sort = c.tock();
  c.ptock();

  // check your work:
  std::cerr << "\nTESTING CORRECTNESS OF MY SORT..." << std::endl;
  for (unsigned long i = 0; i < n; ++i)
    if (x_merge_sort[i] != x_my_sort[i])
      throw std::runtime_error("FAIL: sorted order does not match merge sort");

  std::cerr << "PASSED TESTS OF CORRECTNESS!" << std::endl;

  std::cout << "SPEEDUP: MY VERSION IS " << double(time_merge_sort) / time_my_sort << "x FASTER!" << std::endl;

  delete[] x_merge_sort;
  delete[] x_my_sort;
}

void fill_uniform_random(double *x, unsigned long n)
{
  std::uniform_real_distribution<double> dist(0., 1.);
  // seed our randomness with the clock:
  std::default_random_engine re(std::time(nullptr));
  for (unsigned long i = 0; i < n; ++i)
    x[i] = dist(re);
}

void insertion_sort(double *x, unsigned long n)
{
  for (unsigned long current_key_index = 1; current_key_index < n; ++current_key_index)
  {
    double key = x[current_key_index];
    unsigned long scan = current_key_index - 1;

    
    while (scan >= 0 && x[scan] > key)
    {
      x[scan + 1] = x[scan]; // copy to the right
      --scan;
    }
    x[scan + 1] = key; // put key in correct place
  }
}
unsigned long partition(double *x, unsigned long n)
{
  double pivot_value = x[n / 2];
  double *left = x;          // pointer to first element
  double *right = x + n - 1; // pointer to last element
  while (true)
  {
    // move left pointer forward until >= pivot
    while (*left < pivot_value)
      ++left;
    // move right pointer backward until <= pivot
    while (*right > pivot_value)
      --right;
    if (left >= right)
      return left - x; // pivot index
    // swap values
    double tmp = *left;
    *left = *right;
    *right = tmp;

    ++left;
    --right;
  }
}
void my_sort(double *x, unsigned long n)
{
  if (n <= 128)
  {
    insertion_sort(x, n);
    return;
  }

  unsigned long pivot_index = partition(x, n);

  my_sort(x, pivot_index);
  my_sort(x + pivot_index, n - pivot_index);
}

int main(int argc, char **argv)
{
  if (argc == 2)
  {
    const unsigned long N = atoi(argv[1]);

    double *x = new double[N];
    fill_uniform_random(x, N);
    std::cout << "SORTING " << x[0] << " ";
    if (N > 1)
      std::cout << x[1] << " ";
    if (N > 3)
      std::cout << "... ";
    if (N > 2)
      std::cout << x[N - 1] << std::endl;

    time_my_sort_and_compare_to_merge_sort(x, N);

    delete[] x;
  }
  else
    std::cerr << "Usage: sort <n>" << std::endl;
  return 0;
}

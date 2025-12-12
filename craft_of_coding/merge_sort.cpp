void merge_sort(unsigned long n, unsigned long *source)
{
  if (n <= 1)
  {
    return;
  }

  unsigned long j_cut_off = n - n / 2;
  unsigned long *source2 = source + n / 2;

  merge_sort(n / 2, source);
  merge_sort(j_cut_off, source2);

  // #change int
  unsigned long *buffer = new unsigned long[n];
  unsigned long buffer_int = 0;
  unsigned long i = 0;
  unsigned long j = 0;
  while (i < n / 2 && j < j_cut_off)
  {
    if (source[i] < source2[j])
    {

      buffer[buffer_int] = source[i];
      ++i;
    }
    else
    {
      buffer[buffer_int] = source2[j];
      ++j;
    }
    ++buffer_int;
  }

  for (; i < n / 2; ++buffer_int)
  {
    buffer[buffer_int] = source[i];
    ++i;
  }
  for (; j < j_cut_off; ++buffer_int)
  {
    buffer[buffer_int] = source2[j];
    ++j;
  }

  for (unsigned long k = 0; k < n; ++k)
  {
    source[k] = buffer[k];
  }
  delete[] buffer;
}

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <array_size>\n";
    return 1;
  }

  unsigned long size = std::strtoul(argv[1], nullptr, 10);
  if (size == 0)
  {
    std::cerr << "Array size must be greater than 0.\n";
    return 1;
  }

  unsigned long *arr = new unsigned long[size];

  std::srand(static_cast<unsigned>(std::time(nullptr)));

  // Fill array with random numbers
  for (unsigned long i = 0; i < size; ++i)
  {
    arr[i] = std::rand() % 1000000; // values between 0 and 999999
  }

  std::cout << "Original array: ";
  for (unsigned long i = 0; i < size; ++i)
  {
    std::cout << arr[i] << " ";
  }
  std::cout << "\n";

  // Measure execution time
  auto start = std::chrono::high_resolution_clock::now();
  merge_sort(size, arr);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed = end - start;

  std::cout << "Sorted array:   ";
  for (unsigned long i = 0; i < size; ++i)
  {
    std::cout << arr[i] << " ";
  }
  std::cout << "\n";

  std::cout << "Sorting " << size << " elements took "
            << elapsed.count() << " seconds.\n";

  delete[] arr;
  return 0;
}
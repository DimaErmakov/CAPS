#include <iostream>
void selection_sort(unsigned long n, unsigned long *source)
{
  // change unsigned long

  for (unsigned long i = 0; i < n; ++i)
  {

    for (unsigned long j = i + 1; j < n; ++j)
    {
      if (source[i] > source[j])
      {
        std::swap(source[j], source[i]);
      }
    }
  }
}

void selection_sort(unsigned long n, unsigned long *source);

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
  selection_sort(size, arr);
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
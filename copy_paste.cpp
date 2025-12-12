void insertion_sort(double *x, unsigned long n)
{
  for (unsigned long i = 1; i < n; ++i)
  {
    double key = x[i];
    long j = i - 1;
    while (j >= 0 && x[j] > key)
    {
      x[j + 1] = x[j];
      --j;
    }
    x[j + 1] = key;
  }
}
void my_sort_with_buffer(double *x, double *buffer, unsigned long n)
{
  if (n <= 32)
  {
    insertion_sort(x, n);
    return;
  }

  unsigned long n_divided_by_two = n / 2;
  double *source_2 = x + n_divided_by_two;
  my_sort_with_buffer(x, buffer, n_divided_by_two);
  my_sort_with_buffer(source_2, buffer, n - n_divided_by_two);

  double *left_element = x;
  double *right_element = source_2;
  double *buffer_iterator = buffer;
  while (left_element < x + n_divided_by_two && right_element < x + n)
  {
    bool left_element_if_true = (*left_element < *right_element);

    *buffer_iterator = *left_element * left_element_if_true + *right_element * (!left_element_if_true);

    left_element += left_element_if_true;
    right_element += !left_element_if_true;
    ++buffer_iterator;
  }
  while (left_element < x + n_divided_by_two)
    *buffer_iterator++ = *left_element++;
  while (right_element < x + n)
    *buffer_iterator++ = *right_element++;

  for (unsigned long i = 0; i < n; ++i)
    x[i] = buffer[i];
}
void my_sort(double *x, unsigned long n)
{
  double *buffer = (double *)malloc(n * sizeof(double));

  my_sort_with_buffer(x, buffer, n);

  free(buffer);
}

unsigned long partition(double *x, unsigned long n)
{
  double pivot_value = x[n / 2];
  unsigned long left = 0;
  unsigned long right = n - 1;

  while (true)
  {
    // Find left > pivot
    while (x[left] < pivot_value)
      ++left;
    // Find right < pivot
    while (x[right] > pivot_value)
      --right;
    if (left >= right)
      return left;
    double tmp = x[left];
    x[left] = x[right];
    x[right] = tmp;

    ++left;
    --right;
  }
}

unsigned long partition(double *x, unsigned long n)
{
  double pivot_value = x[n / 2];
  double *left = x;          // pointer to first element
  double *right = x + n - 1; // pointer to last element
  while (true)
  {
    // move left pointer forward until we find >= pivot
    while (*left < pivot_value)
      ++left;
    // move right pointer backward until we find <= pivot
    while (*right > pivot_value)
      --right;
    if (left >= right)
      return left - x;
    // swap values
    double tmp = *left;
    *left = *right;
    *right = tmp;

    ++left;
    --right;
  }
}
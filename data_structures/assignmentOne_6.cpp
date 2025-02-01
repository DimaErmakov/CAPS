/*Describe the action of the new operator. What does it return? What are the indications of
errrors?


The `new` operator in C++ is used for dynamic memory allocation. It is used to allocate memory for a single variable or an array of variables on the heap. When you use the `new` operator, it allocates the required amount of memory and returns a pointer to the beginning of that allocated memory. The syntax for using the `new` operator is as follows:

Here, `type` is the data type of the variable, and `size` is the number of elements in the array.

For example:

```cpp
int* p = new int;        // Allocates memory for a single int
double* arr = new double[10];   // Allocates memory for an array of 10 doubles
```

The `new` operator returns a pointer to the allocated memory, which is then stored in the pointer variable.

Indications of errors related to dynamic memory allocation using `new` may include:

1. **Memory Allocation Failure**: If the system is unable to allocate the requested memory, the `new` operator can throw a `std::bad_alloc` exception. It is important to check for this exception or use `std::nothrow` to handle allocation failure gracefully.

    ```cpp
    int* p = new(std::nothrow) int; // Does not throw an exception on failure
    if (p == nullptr) {
        // Handle memory allocation failure
    }
    ```

2. **Memory Leak**: Forgetting to use the corresponding `delete` operator to deallocate memory can lead to memory leaks. This occurs when allocated memory is not properly released, causing the program's memory usage to grow over time.

It's crucial to manage dynamic memory carefully to avoid memory leaks and to handle allocation failures appropriately. Additionally, modern C++ encourages the use of smart pointers (e.g., `std::unique_ptr` and `std::shared_ptr`) to automate memory management and reduce the risk of memory-related errors.


*/
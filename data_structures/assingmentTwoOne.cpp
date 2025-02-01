/* Write a C++ function which takes an array of int as a parameter and returns the largest
element in the array.Pass the array using a pointer and remember the name of the array automatically
converts to a pointer to the first element of the array.Note that, unlike vectors and strings, arrays do
not carry their size with them.So, you’ll have to also pass an int parameter that tells the function how
many elements are in the array.In the function, iterate through the array using a pointer.Do not index
it.Access the array elements by dereferencing the pointer*/

#include <iostream>
#include <vector>
#include <string>

int largest_element( int* arr, int size);

int main() {
	const int size = 9;
	int arr[size] { 20, 2, 3, 4, 5, 10, 7, 8, 9 };
	std::cout << "The largest element in the array is: " << largest_element(arr, size) << std::endl;
}

int largest_element(int* arr, int size) {
	int* largestNum{arr};
	for (int* i{ arr +1}; i < arr + size; i++) {
		if (*i > *largestNum) {
			largestNum = i;
		}
	}
	return *largestNum;
}
/*If we have the declaration int data[100]; then explain what the expression data + 4
represents? What is its value?

data + 4 represents the address of the 5th element in the array. The value of data + 4 is the address of the 5th element in the array.


*/
/*
#include <iostream>

int main() {
	int data[100];
	std::cout << data + 4 << std::endl;
	std::cout << &data[4] << std::endl;
	std::cout << &data[0] << std::endl;
	return 0;
}
*/
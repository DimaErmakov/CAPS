#include <iostream>
#include <vector>

/*
int& = value (reference)
int* = address
&x = address of x
*x = value of x
*/

//void swap(int& x, int& y);
void swap(int* a, int* b);
int num_greater_than(std::vector<int>& v, int x);
// should be references and const
int main() {
	int x{ 4 };
	int y{ 5 };
	swap(&x, &y);
	std::cout << x << std::endl;
	std::cout << y << std::endl;
	std::vector<int> data{ 1, 2, 3, 4, 5 };
	std::cout << num_greater_than(data, 3);

	return 0;
}

Node* search_list(int key) {
	Node* iter;
	for (iter = head; iter != nullptr; iter = iter->next) {
		if (iter->data == key) {
			return iter;
		}
	}
}
void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
int* give_pointer() {
	int x{ 5 };
	int* p{ new int };
	int* q{ &x };
	// x is memory on stack so it will be deleted when the function ends so should not be returned
	return p;
}
int num_greater_than(const std::vector<int>& v, int x) { // & v is a reference for memeory efficiency, const reference because we don't want to change the vector
	int count{ 0 };
	for (int i = 0; i < v.size(); i++) {
		if (v[i] > x) {
			count++;
		}
	}
	//vec.push_back(99);
	return count;
}
/*
void swap(int& x, int& y) {
	int temp = x;
	x = y;
	y = temp;
}
*/
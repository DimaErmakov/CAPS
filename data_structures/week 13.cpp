#include<vector>
#include<string>
#include<iostream>
#include<memory>

int* ptr_factory(int x) {
	return new int(x);
}

void ptr_use_factory(int x) {
	int* ptr = ptr_factory(x);
	// use the ptr but don't delete it
} // p out of scope, memory leak

std::shared_ptr<int> factory(int x) {
	return std::make_shared<int>(x); // shared_ptr is a smart pointer that retains shared ownership of an object through a pointer
}

void use_factory(int x) {
	std::shared_ptr<int> p = factory(x);
	// use the shared_ptr, no need to delete it
} // p out of scope, memory is automatically freed

std::unique_ptr<int> factory_unique(int x) {
	std::unique_ptr<int> ret(new int(x));
	return ret;
}

int main() {
	std::shared_ptr<std::string> p1;
	p1 = std::make_shared<std::string>("Hello");

	std::cout << *p1 << std::endl;
}
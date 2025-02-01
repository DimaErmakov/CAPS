/*
#include<iostream>
#include<string>
#include<vector>
int main() {
	int* p = new int;
	*p = 5;
	//p is an address
	std::cout << *p << std::endl; // value
	std::cout << p << std::endl; // address
	delete p;
	std::vector<int> data{ 1,2,3,4,5 };
	data.push_back(6);
	std::cout << data[5] << std::endl;
	for (std::vector<int>::size_type i{ 0 }; i < data.size(); i++) {
		std::cout << data[i] << " " << std::endl;
	}
	*/
	/*
	int numbers[5]{ 1,2,3,4,5};
	std::cout << numbers[1] << std::endl; // this is the same as line 7
	std::cout << *(numbers + 1) << std::endl; // this is the same as line 6
	int *add{ &numbers[0] };
	int &a{ numbers[0] };
	std::cout << add << std::endl;
	std::cout << a << std::endl;
	// references are just pointers that are automatically dereferenced
	for (int& num : numbers) {
		num = 2 * num;
	}

	for (int i = 0; i < 5; i++) {
		std::cout << *(numbers+i) << std::endl;
	}
	
	int numbers[5]{ 1,2,3,4,5 };
	for (int* ptr{ numbers }; ptr < numbers + 5; ptr++) {
		std::cout << &ptr << std::endl; // address of ptr
		std::cout << ptr << std::endl; // address of ptr
		std::cout << *ptr << std::endl; // value of ptr
	}

	for (int& ptr : numbers) {
		std::cout << &ptr << std::endl; // address of ptr
		std::cout << ptr << std::endl; // value of ptr
		//std::cout << *ptr << std::endl;
	}
	std::string name;
	std::cout << "Enter your name: ";
	std::cin >> name;
	std::cout << "Hello, " << name << "!" << std::endl;
	
	std::cout << "Enter yeat of college: ";
	int year{ 0 };
	std::cin >> year;
	const int currentYear = 2018; // magic numbers are bad, 2018 is a magic number since it is not clear what it means
	int collegeAge = currentYear - year; // magic numbers are bad, 2018 is a magic number since it is not clear what it means
	std::cout << "You are " << collegeAge << " years old.";
	int arr[9] { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	double avg{ 0 };
	bool is_even{ false };
	int num_entries = 9;
	for (int a = 0; a < num_entries;a++) {
		avg += arr[a];
	}
	avg /= num_entries;
	std::cout << "Average is " << avg << std::endl;
	while (is_even == false) {
		std::cout << "Enter a number: ";
		int num{ NULL };
		std::cin >> num;
		if (num % 2 == 0) {
			is_even = true;
		}
		else {
			std::cout << "Number is not even" << std::endl;
		}
	}
	std::cout << "Enter 5 numbers: ";
	int arr2[5];
	for (int a = 0; a < 5; a++) {
		std::cin >> arr2[a];
	}
	double standard_deviation{ 0.0 };
	
	return 0;

}
*/
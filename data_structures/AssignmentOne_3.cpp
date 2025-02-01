/*
 Represent a zip code as 27 bits. The first and last bit are always 1. The remaining 25 bits
are divided into 5-bit fields. In each field exactly two bits are 1. They are converted to decimal numbers
using the following weightings: 7 4 2 1 0. For example, the 5-bit field 10100 yields 7 + 2 = 9 and the 5-bit
field 00110 yields 2 + 1 = 3. The digit 0 is represented by the weightings that sum to 11; that is, 11000 =
7 + 4 = 11 is the representation for 0.
Write a C++ program that reads in a zip code as a string of 27 bits, converts it to an array of ints,
determines whether the zip code is valid, translates to the actual zip code, and outputs the result as a
string.
The following may be useful. The string method std::stoi() converts a string to an integer. The function
to_string() will convert an integer to a string. The string function substr(pos, length) returns the
substring starting at index pos of a given length (note this is different than the parameters used when
slicing strings in Python). The .length() method returns the number of characters in a string

0 is represented by 11000
9 is represented by 10100
8 is represented by 10010
7 is represented by 10001
6 is represented by 01100
5 is represented by 01010
4 is represented by 01001
3 is represented by 00110
2 is represented by 00101
1 is represented by 00011
*/

#include <iostream>;
#include <string>;

int main() {
	std::cout << "Enter zip code: ";
	std::string zipCode{ "" };
	std::cin >> zipCode;
	int zipCodeArray[27];
	for (int i{ 0 }; i < 27; i++) {
		zipCodeArray[i] = std::stoi(zipCode.substr(i, 1));
	}
	int convertionCode[5]{ 7, 4, 2, 1, 0, };
	std::string finalAnswer{""};
	if (zipCodeArray[0] != 1 || zipCodeArray[26] != 1) {
		std::cout << "Invalid zip code for 1st or 26th = " << zipCodeArray[0] << " " << zipCodeArray[26];
		return -1;
	}
	int oneCount{ 0 };
	for (int i{ 0 }; i < 5; i++) {
		int sum{ 0 };
		for (int j{ 1 }; j < 6; j++) {
			if (zipCodeArray[i * 5 + j] == 1) {
				sum += convertionCode[j-1];
				oneCount++;
			}
		}
		if (sum == 11) {
			sum = 0;
		}
		finalAnswer += std::to_string(sum);
		if (oneCount != 2) {
			std::cout << "Invalid zip code.";
			return -2;
		}
		int& pos = oneCount;
		pos = 0;
	
	}
	std::cout << finalAnswer;
	return 0;
}

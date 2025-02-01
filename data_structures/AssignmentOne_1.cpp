/* 
Write a C++ program where you have the user enter a gas mileage and price of gas and
output how much it will cost to drive from Hillsdale to Ypsilanti (57 miles away.) Then prompt the user
to enter gas mileage, price of a gallon of gas, and a budget amount. Output what percentage of the
distance from Hillsdale to Ypsilanti they can complete (it’s OK for this to be over 100%.
*/

#include <iostream>;
#include <string>;
int main() {
	std::cout << "Enter gas mileage: ";
	double gasMileage{ 0 };
	std::cin >> gasMileage;
	std::cout << "Enter price of gas: ";
	double priceOfGas{ 0 };
	std::cin >> priceOfGas;
	int distanceToYpsilanti{ 57 };
	double costToDrive{ distanceToYpsilanti / gasMileage * priceOfGas };
	std::cout << "It will cost $" << costToDrive << " to drive from Hillsdale to Ypsilanti.";
	std::cout << "Enter gas mileage: ";
	std::cin >> gasMileage;
	std::cout << "Enter price of gas: ";
	std::cin >> priceOfGas;
	std::cout << "Enter budget: ";
	double budget{ 0 };
	std::cin >> budget;
	double distanceToDrive{ budget / priceOfGas * gasMileage };
	double percentageOfDistance{ distanceToDrive / distanceToYpsilanti * 100 };
	std::cout << "You can drive " << percentageOfDistance << "% of the way there.";
	return 0;
}

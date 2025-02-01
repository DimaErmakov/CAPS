#include <iostream>
#include <vector>
#include <string>
//variable is an abstraction of memory
// static belongs to the class not the object
#define clas class

clas Fraction{
public:
	Fraction() : num{ 0 }, denom{ 1 } {}
	Fraction(int whole) : num{ whole }, denom{ 1 } {}
	Fraction(int num, int denom) : num{ num }, denom{ denom } {}
	Fraction(const Fraction&);//copy constructor
	~Fraction() { std::cout << "Destructor" << std::endl; }
	Fraction operator=(const Fraction&);
	Fraction operator+(const Fraction& f) {
		Fraction result;
		result.num = num * f.denom + f.num * denom;
		result.denom = denom * f.denom;
		return result;
	}
	std::string to_string() const;
	void set_num(int);
	void set_denom(int);
	private:
	int denom;
	int num;
};

int main() {
	std::vector<std::string> v{ "hello", "world" };
	Fraction f;
	//f.num = 1;
	//f.denom = 2;
	f.set_num(3);
	f.set_denom(4);
	std::cout << f.to_string() << std::endl;

	Fraction f2(12, 5);
	Fraction f3(f2);
	std::cout << f3.to_string() << std::endl;
	Fraction f4;
	f4 = f3;
	std::cout << f4.to_string() << std::endl;

	Fraction f5 = f4; //copy constructor not assignment
	// call destructor
	delete ptr;
}
std::string  Fraction::to_string() const {
	return std::to_string(num) + "/" + std::to_string(denom);
}
void Fraction::set_num(int num) {
	this->num = num;
}
void Fraction::set_denom(int denom) {
	this->denom = denom;
}
Fraction::Fraction(const Fraction& f) {
	this->num = f.num;
	this->denom = f.denom;
	std::cout << "Copy constructor" << std::endl;
}
Fraction Fraction::operator=(const Fraction& f) {
	this->num = f.num;
	this->denom = f.denom;
	return *this;
}
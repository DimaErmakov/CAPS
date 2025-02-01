#include <iostream>

class A {
public:
	void rint() {
		std::cout << x << std::endl;
	}
	int x = 5;
};

class B : virtual public A {
public:
	void print() {
		std::cout << y << std::endl;
	}
	int y = 10;
};
class C : virtual public A { // virtual inheritance, only one copy of x
	void print() {
		std::cout << z << std::endl;
	}
	int z = 15;
};
class D : public B, public C {
	void print() {
		std::cout << w << std::endl;
	}
	int w = 20;
};
int main() {
	D d;
	d.B::print(); // d has two copies of x
	//d.C::print();
	//d.A::print();
}
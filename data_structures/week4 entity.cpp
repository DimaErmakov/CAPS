#include <iostream>
#include <vector>
#include <string>

class Entity {
public:
	Entity() {}
	Entity(int value);
	Entity(const Entity&);
	void make_value(int value);
	void set_value(int value) const { *data = value; }
	int get_value() const { return *data; }
	void release() { delete data; };
	void operator=(const Entity& other) {
		//data = other.data;
		*(this->data )= *other.data;
	}
private:
	int* data;
};
template<class T>
class Cell{
public:
	Cell(): data{0} {}
	Cell(T value): data{value} {}
	T get_data() const { return data; }
	void set_data(T value) { data = value; }
private:
	T data;
};

int main() {
	Entity a{ 4 };
	Entity b{ 5 };
	b = a; // copy on stack a points to the same address as b
	std::cout << a.get_value() << std::endl;
	std::cout << b.get_value() << std::endl;

	b.set_value(7);
	std::cout << a.get_value() << std::endl;
	std::cout << b.get_value() << std::endl;

	b.release();
	a.release();

	Cell <int> c{ 9 };
	Cell < std::string> d{ "Hello" };
	return 0;
}
Entity::~Entity() {
	delete data;
}
Entity::Entity(int value) {
	make_value(value);
}
void Entity::make_value(int value) {
	data = new int;
	*data = value;
}
Entity::Entity(const Entity& other) {
	data = new int;
	*data = *other.data;
}
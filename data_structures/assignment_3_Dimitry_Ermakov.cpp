/*Write a C++ program to simulate students walking about campus adding and removing items from their
backpacks.
Write three classes: Item, Backpack, and Student. For all of them, write appropriate constructors and
use software development principles involving encapsulation (so write accessors and mutators instead
of exposing the implementation, etc.) Write all the functions listed below but be aware this is not an
exhaustive specification. You may write any additional helper functions you need for your design.
An Item should have a name, a weight, and 4-5 qualities of your choosing (size, type, is_glass,
might_explode, etc.). Write a to_string() method to capture the item and its qualities.
A Backpack should have a capacity and contents. Write contents as a 10-element array of Item pointers.
The add() method should use one of these pointers to dynamically allocate memory to put a new Item
onto the heap. If there are already 10 elements in the array, or if the weight of the Item would exceed
the capacity of the Backpack, then add() should not add the item. The remove() method should take in
an item name and deallocate the memory associated with it. Write a destructor that will deallocate all
the memory. Write a to_string() function that returns the contents of the Backpack.
A Student should have a name, 3-4 other attributes as you see fit, and a Backpack. A Student should be
able to add() and remove() items from their Backpack as well as give() an Item to another Student.
Write a copy constructor for Student that can create a new Student as a copy of an existing Student.
The to_string() method for Student should give the name, a couple attributes, and the contents of the
Backpack.
Write driver code in main() that demonstrates all the functions in action.
*/


// Dimitry Ermakov Assignment 3
#include <iostream>
#include <string>

class Item
{
public:
	Item(std::string name, int weight, bool might_explode, bool is_glass, double price) : name{ name },
		weight{ weight }, might_explode{ might_explode }, is_glass{ is_glass }, price{ price } {}
	std::string to_string() const
	{
		std::string result = "Name: " + name + "\n";
		result += "Weight: " + std::to_string(weight) + "\n";
		result += "Might explode: " + std::to_string(might_explode) + "\n";
		result += "Is glass: " + std::to_string(is_glass) + "\n";
		result += "Price: " + std::to_string(price) + "\n";
		return result;
	}
	int get_weight() const
	{
		return weight;
	}
	std::string get_name() const
	{
		return name;
	}

private:
	std::string name;
	int weight;
	bool might_explode;
	bool is_glass;
	double price;
};
class Backpack
{
public:
	Item* get_item(int index) const
	{
		if (index < 0 || index >= 10)
		{
			return nullptr; // Out of bounds
		}
		return contents[index];
	}
	Backpack(int cap) : capacity{ cap }
	{
		for (int i = 0; i < 10; ++i)
		{
			contents[i] = nullptr;
		}
	}

	~Backpack()
	{
		for (int i = 0; i < 10; ++i)
		{
			delete contents[i];
		}
	}

	int get_total_current_weight() const
	{
		int total_weight = 0;
		for (int i = 0; i < 10; ++i)
		{
			if (contents[i] != nullptr)
			{
				total_weight += contents[i]->get_weight();
			}
		}
		return total_weight;
	}

	int get_capacity() const
	{
		return capacity;
	}

	bool add(Item& item)
	{
		if (get_total_current_weight() + item.get_weight() > capacity)
		{
			return false;
		}

		for (int i = 0; i < 10; ++i)
		{
			if (contents[i] == nullptr)
			{
				contents[i] = new Item(item);
				return true;
			}
		}

		return false; // Backpack is full
	}

	bool remove(const std::string& name)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (contents[i] != nullptr && contents[i]->get_name() == name)
			{
				delete contents[i];
				contents[i] = nullptr;
				return true;
			}
		}
		return false;
	}

	std::string to_string() const
	{
		std::string result = "Capacity: " + std::to_string(capacity) + "\n";
		for (int i = 0; i < 10; ++i)
		{
			if (contents[i] != nullptr)
			{
				result += contents[i]->to_string() + "\n";
			}
		}
		return result;
	}

private:
	int capacity;
	Item* contents[10];
};

class Student
{
public:
	Student(std::string name, std::string major, int age, bool is_tony, int cap)
		: name{ name }, major{ major }, age{ age }, is_tony{ is_tony },
		backpack{ Backpack(cap) } {}
	Student(const Student& other)
		: name{ other.name }, major{ other.major }, age{ other.age },
		is_tony{ other.is_tony }, backpack{ other.backpack.get_capacity() }
	{
		// Copy backpack contents
		for (int i = 0; i < 10; ++i)
		{
			if (other.backpack.get_item(i) != nullptr)
			{
				backpack.add(*(other.backpack.get_item(i)));
			}
		}
	}
	void add_item(Item& item)
	{
		backpack.add(item);
	}

	void remove_item(const std::string& itemName)
	{
		backpack.remove(itemName);
	}

	void give_item(Student& other, Item& item)
	{
		other.add_item(item);
		remove_item(item.get_name());
	}

	std::string get_name() const
	{
		return name;
	}

	std::string get_major() const
	{
		return major;
	}

	int get_age() const
	{
		return age;
	}

	bool get_is_tony() const
	{
		return is_tony;
	}
	Backpack get_backpack() const
	{
		return backpack;
	}

	std::string to_string() const
	{
		std::string result = "Name: " + name + "\n";
		result += "Major: " + major + "\n";
		result += "Age: " + std::to_string(age) + "\n";
		result += "Is Tony: " + std::to_string(is_tony) + "\n\n";
		result += "Backpack:\n" + backpack.to_string() + "\n";
		return result;
	}

private:
	std::string name;
	std::string major;
	int age;
	bool is_tony;
	Backpack backpack;
};

int main()
{
	Item item1("Book", 2, false, false, 10.99);
	Item item2("Laptop", 5, false, false, 999.99);

	Student student1("Alice", "Computer Science", 20, false, 20);

	student1.add_item(item1);

	std::cout << "Student information:\n"
		<< student1.to_string() << std::endl;

	Student student2("Bob", "Engineering", 22, true, 20);

	student1.give_item(student2, item1);

	Student student3(student2);
	student3.add_item(item2);
	std::cout << "Student 3 information (copy of student 2):\n"
		<< student3.to_string() << std::endl;
	std::cout << "Student 2 information:\n"
		<< student2.to_string() << std::endl;

	return 0;
}

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
/*Work with inheritance and polymorphism by setting up a base class with at least one virtual
function and several derived classes and running a main() function that calls the virtual function to
exhibit polymorphism
create a text file of info for, Books, Pens, and at least
one other Item and have the Bookstore read from the files into the inventory. Have the Bookstore able
to print_inventory into a file that includes all the to_string() info from all the Items in the inventory. You
wouldn’t have to add the Customer yet that comes to buy Items as that will be the next step we do in
class
*/
class Item {
public:
	Item() : inventory_id(0), price(0) { std::cout << "Item constructed\n"; }
	Item(int i, int p) : inventory_id(i), price(p) {
		//std::cout << "Item constructed\n";
	}
	virtual std::string to_string() const;
	// virtual function, can be overridden by derived classes
	int get_price() const { return price; }
	int get_inventory_id() const { return inventory_id; }
	bool might_explode = true;
private:
	int inventory_id;
	int price;
};

class Book : public Item {
public:
	//Book() : title("none"), isbn("none"), author("Tony"), pages(0), price(400) {
		//std::cout << "Book constructed\n";
	//}
	Book(int i, int p, std::string t, std::string a, int pg, std::string is)
		: Item(i, p), title(t), author(a), pages(pg), isbn(is) {
		//std::cout << "Book constructed\n";
	}
	void set_isbn(std::string isbn) { this->isbn = isbn; }
	std::string get_isbn() const { return isbn; }
	std::string to_string() const override;
	int get_price() const { return Item::get_price(); }
private:
	int pages;
	std::string title;
	std::string author;
	std::string isbn;
	//int price;
	std::string genre;
	std::string course;
	//int inventory_id;
};
class Pen : public Item {
public:
	Pen() : Item(0, 850), width(0.0), ink_color("fuscia") //price(850), inventory_id(0)
	{
		std::cout << "Pen constructed\n";
	}
	Pen(int i, int p, double w, std::string c) : Item(i, p), width(w), ink_color(c) {
		//std::cout << "Pen constructed\n";
	}
	std::string to_string() const override;
	bool is_metal = true;
private:
	double width;
	std::string ink_color;
	//int price;
	//int inventory_id;
	std::string manufacturer;
	bool might_explode;
};
class Brownies : public Item {
public:
	Brownies() : Item(0, 1000), flavor("good stuff"), size(0.0) {
		std::cout << "Brownies constructed\n";
	}
	Brownies(int i, int p, double s, std::string f) : Item(i, p), size(s), flavor(f) {
		//std::cout << "Brownies constructed\n";
	}
	std::string to_string() const override;

private:
	double size;
	std::string flavor;
};
int main() {
	std::ifstream infile("inventory.txt");
	if (!infile) {
		std::cerr << "Error: Cannot open file 'inventory.txt'" << std::endl;
		return 1;
	}

	std::vector<Item*> inventory;
	std::string line;
	while (std::getline(infile, line)) {
		if (line.empty()) continue; // Skip empty lines
		std::istringstream iss(line);
		std::string type;
		iss >> type;
		std::cout << "Type: " << type << std::endl;
		if (type == "Book") {
			int id, price, pages;
			std::string title, isbn, author;
			std::getline(infile >> std::ws, title);
			std::getline(infile >> std::ws, isbn);
			std::getline(infile >> std::ws, author);
			infile >> pages >> price >> id;
			inventory.push_back(new Book(id, price, title, author, pages, isbn));
		}
		else if (type == "Pen") {
			int id, price;
			double width;
			std::string ink_color;
			infile >> width >> ink_color >> price >> id;
			inventory.push_back(new Pen(id, price, width, ink_color));
		}
		else if (type == "Brownies") {
			int id, price;
			double size;
			std::string flavor;
			std::getline(infile >> std::ws, flavor); // Read multi-word flavor
			infile >> size >> price >> id;
			inventory.push_back(new Brownies(id, price, size, flavor));
		}
		else {
			std::cout << "Unknown type: " << type << std::endl;
		}
	}

	infile.close();

	for (const auto& item : inventory) {
		std::cout << item->to_string() << std::endl;
	}
	std::ofstream outfile("current_inventory.txt");
	if (!outfile) {
		std::cerr << "Error: Cannot open file 'current_inventory.txt'" << std::endl;
		return 1;
	}

	for (const auto& item : inventory) {
		outfile << item->to_string() << std::endl;
	}

	outfile.close();

	return 0;
}
std::string Item::to_string() const {
	std::string result = "Price: " + std::to_string(price) + '\n';
	result += "InventoryID: " + std::to_string(inventory_id) + '\n';
	return result;
}
std::string Book::to_string() const {
	std::string result = "Title: " + title + '\n';
	result += "ISBN: " + isbn + '\n';
	result += Item::to_string() + '\n';
	return result;
}
std::string Brownies::to_string() const {
	std::string result = "Brownies\n";
	result += "Flavor: " + flavor + '\n';
	result += "Size: " + std::to_string(size) + '\n';
	result += Item::to_string() + '\n';
	return result;
}
std::string Pen::to_string() const {
	std::string result = "Pen\n";
	result += "Width: " + std::to_string(width) + '\n';
	result += "Ink color: " + ink_color + '\n';
	result += Item::to_string() + '\n';
	//result += "Price: " + std::to_string(price) + '\n';
	//result += "InventoryID: " + std::to_string(inventory_id) + '\n';
	return result;
}
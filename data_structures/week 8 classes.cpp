#include <iostream>
#include <vector>
#include <string>

#include<iostream>
#include<string>
#include<vector>
class Item {
public:
	Item() : inventory_id(0), price(0) { std::cout << "Item constructed\n"; }
	Item(int i, int p) : inventory_id(i), price(p) {
		std::cout << "Item
			constructed\n"; }
			virtual std::string to_string() const;
		int get_price() const { return price; }
		int get_inventory_id() const { return inventory_id; }
		bool might_explode = true;
private:
	int inventory_id;
	int price;
	};
	class Book : public Item {
	public:
		Book() : title("none"), isbn("none"), author("Tony"), pages(0), price(400) {
			inventory_id = 0;
			std::cout << "Book constructed\n";
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
		int price;
		std::string genre;
		std::string course;
		int inventory_id;
	};
	class Pen : public Item {
	public:
		Pen() : Item(0, 850), width(0.0), ink_color("fuscia") //price(850),
			inventory_id(0)
		{
			std::cout << "Pen constructed\n";
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
	int main() {
		Book book;
		std::cout << book.to_string() << '\n';
		std::cout << book.might_explode << '\n';
		std::cout << book.get_price() << '\n';
		std::cout << book.Item::get_price() << '\n';
		Pen pen;
		std::cout << pen.to_string() << '\n';
		std::cout << pen.get_price() << '\n';
		Item item;
		std::cout << item.to_string() << '\n';
		item = pen;
		std::cout << item.get_price() << '\n';
		std::cout << item.to_string() << '\n';
		//std::cout << item.is_metal
		Item item2;
		std::vector<Item> inventory;
		inventory.push_back(book);
		inventory.push_back(pen);
		for (Item item : inventory) {
			std::cout << item.to_string() << '\n';
		}
		std::vector<Item*> realinventory;
		realinventory.push_back(&book);
		realinventory.push_back(&pen);
		realinventory.push_back(&item2);
		for (Item* item : realinventory) {
			std::cout << item->to_string() << '\n'; // (*item).to_string()
		}
	}
	std::string Item::to_string() const {
		std::string result = "Price: " + std::to_string(price) + '\n';
		result += "InventoryID: " + std::to_string(inventory_id) + '\n';
		return result;
	}
	std::string Book::to_string() const {
		std::string result = "Title: " + title + '\n';
		result += "ISBN: " + isbn + '\n';
		result += "Price: " + std::to_string(price) + '\n';
		result += "InventoryID: " + std::to_string(inventory_id) + '\n';
		return result;
	}
	std::string Pen::to_string() const {
		std::string result = "Width: " + std::to_string(width) + '\n';
		result += Item::to_string() + '\n';
		//result += "Price: " + std::to_string(price) + '\n';
		//result += "InventoryID: " + std::to_string(inventory_id) + '\n';
		result += std::to_string(get_inventory_id()) + '\n';
		return result;
		/*
			virtual std::string to_string() const; // virtual function, can be overriden by subclasses
			// polymorphism, can use subclass objects in place of superclass objects, dynamic binding, automatic selection of the function to call
			//static binding, compiler knows which function to call at compile time
			// dynamic binding, compiler doesn't know which function to call at compile time, so it waits until runtime
			int get_price() const {
				return price;
			}
		protected: //any subclass can access this

			std::string to_string() const override; // just for clarity

		int main() {
			Book b;
			std::cout << b.to_string() << std::endl;
			std::cout << b.get_price() << std::endl;
			std::cout << b.Item::get_price() << std::endl;

			Pen p;
			std::cout << p.to_string() << std::endl;

			std::vector<Item> inventory;
			Item i;
			i = b; // slicing problem, only gets item attributes from book
			Item i2;
			inventory.push_back(b);
			inventory.push_back(p);

			for (Item i : inventory) {
				std::cout << i.to_string() << std::endl;
			} // only gets items attibutes, slicing problem
			//Item i;
			//std::cout << i.to_string() << std::endl;

			std::vector<Item*> realinventory;
			realinventory.push_back(&b);
			realinventory.push_back(&p);
			realinventory.push_back(&i2);

			for (Item* i : realinventory) {
				std::cout << i->to_string() << std::endl; // no slicing problem, -> is used to access the pointer
			}

			return 0;
		}

		*/
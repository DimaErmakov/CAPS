// https://hillsdale.instructure.com/courses/7615/assignments/50549

/* Implement a hash table with linear probing for collision resolution. Create a list of 20 words
and use the standard mod hash function. Build it with an m of 40 and 80. Write this all as a class and
include a find function that will return true or false if a word is in the data structure or not.*/
#include <iostream>
#include <vector>
#include <string>

class HashTable {
public:
	std::string toString() {
		std::string result = "";
		for (int i = 0; i < TABLE_SIZE; i++) {
			result += table[i] + "\n";
		}
		return result;
	}
	HashTable(int size) {
		TABLE_SIZE = size;
		table = new std::string[TABLE_SIZE];
	}

	void insert(const std::string& key) {
		int index = hash(key);
		for (int i = 0; i < TABLE_SIZE; i++) {
			if (table[index] == "") {
				table[index] = key;
				return;
			}
			// Linear probe move to the next slot
			std::cerr << "Collision detected at index " << index << " Trying next slot" << std::endl;
			index = (index + 1) % TABLE_SIZE;
		}
		std::cerr << "Unable to insert " << key << " Table is full" << std::endl;
	}

	bool find(const std::string& key) {
		int index = hash(key);
		while (table[index] != "") {
			if (table[index] == key) {
				return true; // Key found
			}
			index = (index + 1) % TABLE_SIZE; // Move to the next slot
		}
		return false; // Key not found
	}

	int hash(const std::string& word) {
		int sum = 0;
		for (char c : word) {
			sum += static_cast<int>(c); // Convert char to ASCII
		}
		return sum % TABLE_SIZE;
	}
private:
	int TABLE_SIZE;
	std::string* table;
};

int main() {
	HashTable table(40);
	std::string words[20] = { "kdo", "my", "cat", "cat", "sss", "fish", "fisha", "horse", "e", "saasaasasasasas",
							 "mdkmfkmdfefefkj", "lion", "monkfey", "noodlse",
							"doc", "pen", "fkkks", "rabbit", "snake", "tiger" };

	for (const auto& word : words) {
		table.insert(word);
	}

	std::cout << "Check 'cat' in table: " << (table.find("cat") ? "Yes" : "No") << std::endl;
	std::cout << "Check 'zebra' in table: " << (table.find("zebra") ? "Yes" : "No") << std::endl;

	std::cout << table.toString();

	HashTable table_two(80);

	for (const auto& word : words) {
		table_two.insert(word);
	}

	std::cout << "Check 'cat' in table: " << (table_two.find("cat") ? "Yes" : "No") << std::endl;
	std::cout << "Check 'zebra' in table: " << (table_two.find("zebra") ? "Yes" : "No") << std::endl;

	std::cout << table_two.toString();

	return 0;
}
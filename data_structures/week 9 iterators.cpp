#include <map>
#include <iostream>
#include <string>
int main() {
	std::map<std::string, std::string> planets;
	planets["Mercury"] = "hot";
	planets["Venus"] = "hoter";
	planets["Earth"] = "just right";

	/*if (planets.find("Earth") != planets.end()) {
		std::cout << "Earth is " << planets["Earth"] << std::endl;
	}*/
	for (auto it = planets.begin(); it != planets.end(); it++) {
		std::cout << it->first << " is " << it->second << std::endl;
	}
}
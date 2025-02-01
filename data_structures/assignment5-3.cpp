#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <list>
#include <iterator>
/*You have collected a file of movie ratings where each movie is rated from 1 (bad) to 5
(excellent). The first line of the file is a number that identifies how many ratings are in the file.
Each rating then consists of two lines: the name of the movie followed by the numeric rating from 1 to
Use a C++ map to store the data
Write a program that reads in a file in this format (or you may initialize a vector of strings with the data
and hardcode it), stores the data in a map, and then calculates the average rating for each movie and
outputs the average along with the number of reviews for each
*/

int main() {
	std::map<std::string, std::list<int>> movies;
	std::ifstream inputFile("movies.txt");
	std::string line1, line2;
	int numRatings;
	inputFile >> numRatings;
	inputFile.ignore(); // Ignore the newline character after number of ratings
	for (int i = 0; i < numRatings; i++) {
		std::getline(inputFile, line1);
		std::getline(inputFile, line2);
		if (movies.find(line1) == movies.end()) {
			movies[line1] = { std::stoi(line2) }; // create a new list
		}
		else {
			movies[line1].push_back(std::stoi(line2)); // add to list
		}
	}
	for (auto movie : movies) {
		std::cout << "Movie: " << movie.first << std::endl;
		std::cout << "Number of reviews: " << movie.second.size() << std::endl;
		double total = 0;
		for (auto rating : movie.second) {
			total += rating;
		}
		std::cout << "Average rating: " << total / movie.second.size() << std::endl;
	}

	return 0;
}
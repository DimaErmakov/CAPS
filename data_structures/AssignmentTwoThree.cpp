/* Write a C++ program that plays the game of Nim. In Nim, there is a pile of tokens and each
player takes turns removing tokens. A player must take at least one token but cannot remove more than
half the tokens in one turn and the player who removes the last token loses the game.
Decompose the problem into three classes: Player, Pile, and Game. Your main function should declare
an object of type Game and call the .play() function of game.

The Player class should consist of a player name and have a make_move() function that takes a Pile as a
parameter and generates a legal move. If the Player name is “Computer” then this move should be
random. If the Player name is anything else, then the move should be input from the keyboard.

The Pile class should contain a number of tokens, a max_legal_move() function that returns the
maximum legal move, a remove_tokens() function that takes a number of tokens as a parameter and
updates the pile accordingly. The constructor should initialize the number of tokens randomly between
50 to 150.

The Game class should contain two Players and a Pile. Its play() function should run through the
gameplay. For each move, output the Player names and how many tokens are left in the Pile. Give
appropriate feedback to the user about the game state, prompt for move, announce the winner, etc.
The Game constructor should take two player names.
In your class design, incorporate principles of encapsulation. Do not expose data members to other
classes. They should be accessed through apprpriate get() and set() methods. Write appropriate
constructors for each class.
*/
#include <vector>
#include <string>
#include <iostream>
#include <random>

int getRandomNumber(int min, int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(gen);
}
class Pile {
public:
	Pile() : tokens{ getRandomNumber(50, 150) } {}
	int max_legal_move() {
		if (this->get_tokens() == 1) {
			return 1;
		}
		return this->get_tokens() / 2; 
	}
	void remove_tokens(int num) {
		this->set_tokens(this->get_tokens() - num);
	}
	void set_tokens(int num) {
		this->tokens = num;
	}
	int get_tokens() {
		return this->tokens;
	}
private:
	int tokens;
};
class Player {
public:
	Player(std::string name) : name(name) {}
	void make_move(Pile& pile) {
		if (this->name == "Computer") {
			int num = getRandomNumber(1, pile.max_legal_move());
			pile.remove_tokens(num);
			std::cout << this->name << " removed " << num << " tokens.\n" << std::endl;
		}
		else {
			std::cout << "Enter number of tokens to remove: " << std::endl;
			int num;
			std::cin >> num;
			if ((num > pile.max_legal_move() || num < 1) && pile.get_tokens() > 1) {
				std::cout << "Invalid move. Try again." << std::endl;
				make_move(pile);
			}
			else {
				pile.remove_tokens(num);
				std::cout << this->name << " removed " << num << " tokens.\n" << std::endl;
			}

		}
	}
	std::string get_name() {
		return this->name;
	}
private:
	std::string name;
};
class Game {
public:
	Game(std::string nameOne, std::string nameTwo) : playerOne(nameOne), playerTwo(nameTwo), pile() {}
	void play() {
		while (pile.get_tokens() > 0) {
			std::cout << playerOne.get_name() << " has " << pile.get_tokens() << " tokens left." << std::endl;
			playerOne.make_move(pile);
			if (pile.get_tokens() == 0) {
				std::cout << playerTwo.get_name() << " wins!" << std::endl;
				break;
			}
			std::cout << playerTwo.get_name() << " has " << pile.get_tokens() << " tokens left." << std::endl;
			playerTwo.make_move(pile);
			if (pile.get_tokens() == 0) {
				std::cout << playerOne.get_name() << " wins!" << std::endl;
				break;
			}
		}
	}
private:
	Player playerOne;
	Player playerTwo;
	Pile pile;
};
int main() {
	Game game("Player", "Computer");
	game.play();
	return 0;
}

/*
Write a number guessing game program in C++ where you prompt the user to enter a
number in a range and output whether the guess is too high or too low until the user guesses correctly.
Keep track of the number of guesses the user input and ask if they want to play the game again. At the
end of the program, after the user has indicated there are no more games to be played, output the
scores from the play session and tell the user what their best score was
*/


#include <iostream>;
#include <string>;

int main() {
	int guess{ 0 };
	int numberOfGuesses{ 1 };
	int bestScore{ 0 };
	bool playAgain{ true };
	while (playAgain) {
		int randomNumber{ rand() % 100 + 1 };
		std::cout << "Enter a number between 1 and 100: \n";
		std::cin >> guess;
		while (guess != randomNumber) {
			if (guess > randomNumber) {
				std::cout << "Too high\n";
			}
			else {
				std::cout << "Too low\n";
			}
			std::cout << "Enter a number between 1 and 100: \n";
			std::cin >> guess;
			numberOfGuesses++;
		}
		std::cout << "Correct\n";
		std::cout << "You guessed " << numberOfGuesses << " times\n";
		if (numberOfGuesses < bestScore || bestScore == 0) {
			bestScore = numberOfGuesses;
		}
		std::cout << "Your best score is " << bestScore << " guesses\n";
		std::cout << "Play again? (y/n): \n";
		std::string playAgainString{ "" };
		std::cin >> playAgainString;
		if (playAgainString == "n") {
			playAgain = false;
		}
		else {
			numberOfGuesses = 1;
		}
	}
}

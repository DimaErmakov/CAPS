/* Write a C++ class that represents a combination lock. It should have a three-digit
combination that is set in the constructor. The device will be unlocked by turning left a number of ticks
equal to the first digit, then right a number of ticks equal to the second digit, and then left again. Any
deviation from this sequence will not unlock the lock and the lock will then need to be reset in order to
make any further progress towards unlocking.
There should be turn_left() and turn_right() functions that take a number of ticks as a parameter and
represent the action of turning the lock left or right as the user tries to open it. An open() function
should return “Open” if the lock is able to be opened and “Locked” if the lock is still locked. (Trying to
open it when it’s still locked does not force the lock to need a reset before further progress can be
made.)
There should be a reset() function that lets the user try to enter the correct combination again after a
failed attempt.
Declare an object of type Lock in your main() function and demonstrate your functions work as they
should.*/

#include <iostream>
#include <vector>
#include <string>

class Lock {
public:
	Lock(int numOne, int numTwo, int numThree) : combination{ numOne, numTwo, numThree } {}

	void turn_left(int ticks) {
		if (ticks == combination[0] && unlockedTwo == false && unlockedThree == false) {
			std::cout << "Left turn successful" << std::endl;
			unlockedOne=true;
		}
		else if (ticks == combination[2] && unlockedTwo && unlockedOne) {
			std::cout << "Left turn successful" << std::endl;
			unlockedThree = true;
		}
		else {
			std::cout << "Left turn unsuccessful" << std::endl;
		}
		
	}
	void turn_right(int ticks) {
		if (ticks == combination[1] && unlockedOne && unlockedThree == false) {
			std::cout << "Right turn successful" << std::endl;
			unlockedTwo=true ;
		}
		else {
			std::cout << "Right turn unsuccessful" << std::endl;
		}

	}
	void reset() {
		unlockedOne = false;
		unlockedTwo = false;
		unlockedThree = false;
	}
	std::string open() {
		if (unlockedOne && unlockedTwo && unlockedThree) {
			return "Open";
		}
		else {
			return "Locked";
		
		}
	}

private:
	int combination[3];
	bool unlockedOne{ false };
	bool unlockedTwo{ false };
	bool unlockedThree{ false };

};

int main() {
	Lock l(36,6,12);

	l.turn_left(36);
	l.turn_right(6);
	l.turn_left(12);
	std::cout << l.open() << std::endl;

	l.reset();
	// Incorrect sequence
	l.turn_left(12);
	l.turn_right(6);
	l.turn_left(36);

	std::cout << "Status: " << l.open() << std::endl;

	// Incorrect ticks
	l.reset();
	l.turn_left(3);
	l.turn_right(5);
	l.turn_left(8);

	std::cout << "Status: " << l.open() << std::endl;

	// Incorrect turns
	l.reset();
	l.turn_left(36);
	l.turn_right(6);
	l.turn_right(12);

	std::cout << "Status: " << l.open() << std::endl;

	return 0;
}
#include <iostream>
#include <vector>
#include <list>
#include <cmath>

class Stack { // index 0 is the top of the stack
public:
    void push(double value) {
        data.push_front(value);
    }
    void pop() {
        data.pop_front();
    }
    bool is_empty() {
        return data.size() == 0;
    }
    void clear() {
        data.clear();
    }
    double peek() {
        return data.front();
    }
    void print_stack() {
        for (double value : data) {
			std::cout << value << " ";
		}
		std::cout << std::endl;
	}

private:
    std::list<double> data;
};

class Math {
public:
    void push(double value) {
        stack.push(value);
    }
    void pop() {
        stack.pop();
    }
    double peek() {
        return stack.peek();
    }
    void add() {
        double num1 = stack.peek();
        stack.pop();
        double num2 = stack.peek();
        stack.pop();
        stack.push(num1 + num2);
    }
    void subtract() {
        double num1 = stack.peek();
        stack.pop();
        double num2 = stack.peek();
        stack.pop();
        stack.push(std::abs(num2 - num1)); 
    }
    void multiply() {
        double num1 = stack.peek();
        stack.pop();
        double num2 = stack.peek();
        stack.pop();
        stack.push(num1 * num2);
    }
    void divide() {
        double num1 = stack.peek();
        stack.pop();
        double num2 = stack.peek();
        stack.pop();
        double remainder = std::fmod(num2, num1);
        if (num1 == 0) {
            std::cout << "Error: Division by zero." << std::endl;
            return;
        }
        stack.push(num2 / num1);
        stack.push(remainder);
    }
    void sin() {
        double num = std::sin(stack.peek());
        stack.pop();
        stack.push(num);
    }
    void sincos() {
        double num = std::sin(stack.peek());
        stack.pop();
        double num2 = std::cos(stack.peek());
        stack.pop();
        stack.push(num2);
        stack.push(num);
    }
    void sqrt() {
        double num = std::sqrt(stack.peek());
        stack.pop();
        stack.push(num);
    }
    void print() {
        stack.print_stack();
	}
     
private:
    Stack stack;
};

int main() {
    Math math;

    math.push(2.0);
    math.push(3.0);
    math.print();
    math.add();
    std::cout << "Addition result: " << math.peek() << std::endl;
    math.pop();
    math.print();

    math.push(5.0);
    math.push(2.0);
    math.print();
    math.subtract();
    std::cout << "Difference result: " << math.peek() << std::endl;
    math.print();

    math.push(3.0);
    math.push(4.0);
    math.print();
    math.multiply();
    std::cout << "Multiplication result: " << math.peek() << std::endl;
    math.print();

    math.push(10.0);
    math.push(2.0);
    math.print();
    math.divide();
    math.print();

    math.push(90.0);
    math.print();
    math.sin();
    std::cout << "Sine result: " << math.peek() << std::endl;

    math.print();
    math.push(80.0);
    math.push(90.0);
    math.print();
    math.sincos();
    math.print();

    math.push(16.0);
    math.print();
    math.sqrt();
    std::cout << "Square root result: " << math.peek() << std::endl;
    math.print();

    return 0;
}

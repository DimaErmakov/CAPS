#include<iostream>
#include<string>
#include<vector>

class Employee {
public:
	Employee(const std::string& f, const std::string& l, const std::string& ssn) :
		firstname(f), lastname(l), ssn(ssn) {}

	virtual double earnings() const = 0; // pure virtual function, abtract class is a class that has at least one pure virtual function
	void print() const {
		std::cout << firstname << ' ' << lastname << "SSN: " << ssn << std::endl;
	}

private:
	std::string firstname;
	std::string lastname;
	std::string ssn;
};

class SalariedEmployee : public Employee {
public:
	SalariedEmployee(const std::string& f, const std::string& l, const std::string& ssn, double salary = 0.0) :
		Employee(f, l, ssn), salary(salary) {}
	virtual double earnings() const {
		return salary;
	}
private:
	double salary;
};

class CommissionEmployee : public Employee {
public:
	CommissionEmployee(const std::string& f, const std::string& l, const std::string& ssn, double sales = 0.0, double rate = 0.0) : // = 0.0 is default value
		Employee(f, l, ssn), grossSales(sales), commissionRate(rate) {}
	virtual double earnings() const {
		return commissionRate * grossSales;
	}
private:
	double grossSales;
	double commissionRate;
};;

int main() {
	std::vector<Employee*> employees;
	employees.push_back(new SalariedEmployee("John", "Smith", "111-11-1111", 800));
	employees.push_back(new CommissionEmployee("Sue", "Jones", "333-33-3333", 10000, 0.06));
	employees.push_back(new Employee("Sue", "Jones", "333-33-3333"));

	for (size_t i = 0; i < employees.size(); i++) {
		std::cout << "earned: " << employees[i]->earnings() << std::endl;
	}
}
/*
If i is an int variable and p and q are pointers to int, which of the following assignments are
legal?
(a) p = i; (d) p = &q; (g) p = *q;
(b) *p = &i; (e) p = *&q; (h) *p = q;
(c) &p = q; (f) p = q; (i) *p = *q;


int* address
&i address (i number)
*p value pointing to (p pointer)
int& value

a illegal address to value
b illegal value to address
c illegal expression must be a modifiable value
d illegal cannot assign int** to int* f is the correct version of d
e legal redundant but pointer to point to address of pointer
f legal address to address 
g illegal cannot assign int* to value
h illegal value to address
i legal value to value
https://hillsdale.instructure.com/courses/7615/assignments/48586


*/
/*
#include <iostream>

int main() {
	int i { 0 };
	int* q;
	int* p;
	*p = *q;
	//*p = 5;
	std::cout << q << std::endl;
	std::cout << p << std::endl;
	std::cout << i << std::endl;
	//int* q { &i };
	//p = i; // illegal

}
*/
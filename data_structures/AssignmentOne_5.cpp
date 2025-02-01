/*(5) (5 pts) Suppose a dynamic variable were created as follows:
char* p;
p = new char;
Assuming that the value of the pointer variable p has not changed, how can you destroy this new
dynamic variable and return the momory it uses so that the memory can be reused to create other new
dynamic variables?


// Deallocate the dynamic variable and release the memory
delete p;


*/
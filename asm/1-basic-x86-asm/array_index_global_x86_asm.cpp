#include <iostream>
#include <stdio.h>

long i;

// array[] lives -80B, -72B, -64B, -56B, -48B, -40B, -32B, -24B, -16B (relative to %rbp)
long array[] = {0,1,2,3,4,5,6,7,8};

int main() {
  std::cout << "pre" << std::endl;
  for (i=0; i<9; ++i)
    std::cout << array[i] << std::endl;
  std::cout << std::endl;
  
  //  array[6] = 100;
  // 6 slots of size 8B is 48B offset:
  asm("movq $100,%rax;"
      "leaq array(%rip),%rbx;"
      "add $48,%rbx;"
      "movq %rax,(%rbx);");

  std::cout << "post" << std::endl;
  for (i=0; i<9; ++i)
    std::cout << array[i] << std::endl;
  std::cout << std::endl;
}

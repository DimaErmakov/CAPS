#include <iostream>
#include <stdio.h>

long i;

int main() {
  // array[] lives -80B, -72B, -64B, -56B, -48B, -40B, -32B, -24B, -16B (relative to %rbp)
  long array[] = {0,1,2,3,4,5,6,7,8};

  for (i=0; i<9; ++i)
    std::cout << array[i] << std::endl;
  std::cout << std::endl;
  
  //  a[3] += 100;
  asm("movq -56(%rbp),%rax;"
      "add $100,%rax;"
      "movq %rax,-56(%rbp);");

  for (int i=0; i<9; ++i)
    std::cout << array[i] << std::endl;
  std::cout << std::endl;
}

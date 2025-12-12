#include <stdio.h>

long get_rbp() {
  register long i asm("rbp");
  return i;
}

int main() {
  printf("rbp: %lx\n", get_rbp());
  
  return 0;
}

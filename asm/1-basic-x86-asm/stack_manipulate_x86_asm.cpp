#include <iostream>
#include <string.h>

// NOTE: locations depend on compiler. may have changed

int main() {
  // x[] lives at -14B, -13B, -12B, -11B, -10B, -9B (relative to %rbp)
  char x[] = {'G','A','T','C','.','.'};

  // So far we've seen 6B of data; we need to round it out to 8B so
  // that we still have alignment (i.e., not using the low byte of one
  // word and the high byte of the next word as a single word)

  // y[] lives at -32B, -28B, -24B, -20B (relative to %rbp)
  int y[] = {1,2,3,4};

  // i lives at -8B (relative to %rbp)
  long i = 7;

  // x[2] += 3
  asm("mov -12(%rbp),%al;"
      "add $3,%al;"
      "mov %al,-12(%rbp)");

  // x[0] = x[3] + 1
  asm("mov -11(%rbp),%al;"
      "add $1,%al;"
      "mov %al,-14(%rbp)");

  std::cout << x[0] << std::endl;
  std::cout << x[1] << std::endl;
  std::cout << x[2] << std::endl;
  std::cout << x[3] << std::endl;
  std::cout << x[4] << std::endl;
  std::cout << x[5] << std::endl << std::endl;

  // i -= 3
  asm("mov -8(%rbp),%rax;"
      "sub $3,%rax;"
      "mov %rax,-8(%rbp)");

  std::cout << i << std::endl;

  return 0;
}

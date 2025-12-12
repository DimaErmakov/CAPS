#include <iostream>
#include <string.h>

void f() {
  std::cout << "f" << std::endl;
  std::cout << "\nWINNER WINNER CHICKEN DINNER!\n" << std::endl;
  std::cout << "/f" << std::endl;
}

std::string input_str;

void g() {
  std::cout << "g" << std::endl;

  // Lives at -8B (relative to %rbp)
  long ret_address = 0;
  asm("movq 8(%rbp),%rax;"
      "movq %rax,-8(%rbp);");
  std::cout << "Ret address (void*): " << (void*)ret_address << std::endl;

  // 7 characters + NULL
  // Lives at -16B, -15B, -14B, -13B, -12B, -11B, -10B, -9B (relative to %rbp)
  char some_string[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  // To start overwriting return address, need to enter 25 characters (including NULL).
  // NOTE: assumes no stack canary, so compile with -fno-stack-protector
  // 8 characters write some_string
  // 8 characters overwrite ret_address
  // 8 characters absorbed by (%rbp)
  // >0 characters write return address at -8(%rbp)
  strcpy(some_string, input_str.c_str());
  
  // Write in a string longer than 8B (including NULL) to overwrite
  // the return address to call f instead of returning to main:

  // Print current return address:
  asm("movq 8(%rbp),%rax;"
      "movq %rax,-8(%rbp);");
  std::cout << "New ret address (void*): " << (void*)ret_address << std::endl;
  
  std::cout << "/g" << std::endl;
}

int main(int argc, char**argv) {
  if (argc != 2) {
    std::cerr << "usage: <input_string>" << std::endl;
    std::cerr << "e.g., ./smash_return_address \"aaaaaaaaaaaaaaaaaaaaaaaa$(../0-utils/ptr2str 0x555555555229)\"" << std::endl;
    std::cerr << "the actual pointer value will be determined by running multiple times. the first time, find the address of f. the second time, use the address of f. note that you'll need to disable ASLR or be very lucky!" << std::endl;
  }
  else {
    input_str = argv[1];
    std::cout << "main" << std::endl;

    std::cout << "(void*) main: " << (void*)main << std::endl;

    std::cout << "(void*) f: " << (void*)f << std::endl;

    g();

    std::cout << "/main" << std::endl;
  }
  return 0;
}

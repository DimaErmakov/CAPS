#include <iostream>

void f() {
  std::cout << "f" << std::endl;
  std::cout << "OOPS! who called f??" << std::endl;
  std::cout << "/f" << std::endl;
}

void g() {
  std::cout << "g" << std::endl;

  void (*func)(void) = f;

  // Overwrite return address (we will go to f instead of returning to main):
  asm("movq -8(%rbp),%rax;"  // %rax <- f
      "movq %rax,8(%rbp);"); // return address <- %rax

  // This is called before calling f:
  std::cout << "/g" << std::endl;
}

int main() {
  std::cout << "main" << std::endl;
  g();

  // This is never called:
  std::cout << "/main" << std::endl;
  return 0;
}

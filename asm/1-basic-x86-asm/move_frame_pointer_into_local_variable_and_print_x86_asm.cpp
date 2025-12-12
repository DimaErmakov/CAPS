#include <iostream>

void f() {
  int (*func)(void) = NULL;

  asm("movq 8(%rbp),%rax;"
      "movq %rax,-8(%rbp);");

  // Return address (now in func) is between starts of g and main:
  std::cout << "Return address of f " << (void*)func << std::endl;
}

void g() {
  f();

  int (*func)(void) = NULL;

  asm("movq 8(%rbp),%rax;"
      "movq %rax,-8(%rbp);");

  // Return address (now in func) is after start of main:
  std::cout << "Return address of g " << (void*)func << std::endl;
}

int main()
{
  std::cout << "f    " << (void*)f << std::endl;
  std::cout << "g    " << (void*)g << std::endl;
  std::cout << "main " << (void*)main << std::endl;
  std::cout << std::endl;
  g();
}

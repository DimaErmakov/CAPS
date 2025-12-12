#include <iostream>

void*temp = 0;
long i;

void f(long a, long b, long c, long d, long e, long f) {
  // return address lives at 8B (relative to %rbp)
  // old %rbp lives at 0 (relative to %rbp)

  long x[] = {1,2,3,4,5,6,7,8,9};
  long y = 0xa;
  long z = 0xb;

  std::cout << "\ninside f, after locals declared:" << std::endl;
  asm("movq %rbp,temp(%rip);");
  std::cout << "%rbp    \t" << temp << std::endl;
  asm("movq %rsp,temp(%rip);");
  std::cout << "%rsp    \t" << temp << std::endl;
  asm("movq (%rsp),%rax;"
      "movq %rax,temp(%rip);");
  std::cout << "(%rsp)   \t" << temp << std::endl;
  asm("movq %rsi,temp(%rip);");
  std::cout << "%rsi    \t" << temp << std::endl;
  asm("movq %rdi,temp(%rip);");
  std::cout << "%rdi    \t" << temp << std::endl;

  
  std::cout << "\nstack inside f (address, relative to %rbp, value):" << std::endl;
  std::cout << "---------------" << std::endl;

  for (i=-20*8; i<=32; i+=8) {
    // Move %rbp+i into temp:
    asm("movq i(%rip),%rax;"
	"addq %rbp,%rax;"
	"movq %rax,temp(%rip);");
    std::cout << temp << "\t" << i << "(%rbp)=" << "\t";
    // Move *(%rbp+i) into temp:
    asm("movq i(%rip),%rax;"
	"addq %rbp,%rax;"
	"mov (%rax),%rax;"
	"movq %rax,temp(%rip);");
    std::cout << temp << std::endl;
  }
}

int main() {
  std::cout << "f        \t" << (void*)f << std::endl;
  std::cout << "main     \t" << (void*)main << std::endl;

  // Not all of these arguments end up on the stack (sometimes
  // registers may be used for arguments).
  f(0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666);

  return 0;
}

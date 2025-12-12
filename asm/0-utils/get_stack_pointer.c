#include <stdio.h>

int main() {
  register long i asm("rsp");

  printf("rsp: 0x%lx\n", i);

  char rsp_str[17];
  *((long*)rsp_str) = i;
  rsp_str[16] = 0;

  printf("rsp as str: %s\n", rsp_str);

  return 0;
}

#include <unistd.h>
#include <stdlib.h>

int main () {
  char* shells[]={"/bin/sh", 0};
  execve(shells[0], &shells[0], NULL);
  return 0;
}

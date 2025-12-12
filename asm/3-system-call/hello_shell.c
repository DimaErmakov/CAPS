#include <unistd.h>
#include <stdlib.h>

const char*shell = "/bin/sh";

int main () {
  system(shell);
  return 0;
}

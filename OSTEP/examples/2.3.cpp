#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// gcc 2.3.cpp -o 2.3.out
int main(int argc, char *argv[]) {
  // malloc(sizeof(int)) : void*
  int *p = (int *)malloc(sizeof(int));
  // Assertion failed: (p != NULL), function main, file 2.3.cpp, line 9.
  // zsh: abort./ 2.3.out
  assert(p != NULL);
  printf("(%d) address pointed to by p: %p\n", getpid(), p);
  *p = 0;
  while (1) {
    Spin(1);
    *p = *p + 1;
    printf("(%d) p: %d\n", getpid(), *p);
  }
  return 0;
}

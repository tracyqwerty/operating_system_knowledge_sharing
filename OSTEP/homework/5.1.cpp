#include <stdio.h>  //printf
#include <stdlib.h> //exit(1), fprintf
#include <unistd.h> //fork
//gcc 5.1.cpp -o 5.1.out
int main() {
  int x = 100;
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("hello, I am child (pid:%d)\n", (int)getpid());
    x = 200;
    printf("child x is %d\n", x);
  } else {
    printf("hello, I am parent of %d (pid:%d)\n", rc, (int)getpid());
    x = 300;
    printf("parent x is %d\n", x);
  }
  return 0;
}
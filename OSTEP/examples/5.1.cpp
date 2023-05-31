#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// gcc 5.1.cpp -o 5.1.out
int main(int argc, char *argv[]) {
  printf("hello world (pid:%d)\n", (int)getpid());
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    // here rc_wait is -1, nothing to wait.
    // int rc_wait = wait(NULL);
    printf("hello, I am child (pid:%d)\n", (int)getpid());
  } else {
    // Adding a wait() call to the code above makes the output deterministic.
    int rc_wait = wait(NULL);
    printf("hello, I am parent of %d (pid:%d)(rc_wait:%d)\n", rc, (int)getpid(),
           rc_wait);
  }

  return 0;
}
#include <stdio.h>  // standard input/output
#include <stdlib.h> // standard library
#include <sys/wait.h>
#include <unistd.h> // standard symbolic constants and types
// gcc 5.5.cpp -o 5.5.out
void check_fork(pid_t rc) {
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    // terminate the process immediately
    exit(1);
  }
}
int main() {
  pid_t rc = fork();
  check_fork(rc);
  if (rc == 0) {
    // int rc_wait = wait(NULL); // -1
    printf("hello, I am child (pid:%d)\n", (int)getpid());
  } else {
    int rc_wait = waitpid(rc, NULL, 0); // rc_wait = child's pid
    printf("hello, I am parent of %d (pid:%d)(rc_wait:%d)\n", rc, (int)getpid(),
           rc_wait);
  }
  return 0;
}
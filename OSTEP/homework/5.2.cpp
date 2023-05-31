#include <assert.h>
#include <fcntl.h>  //open
#include <stdio.h>  //fprintf, stderr
#include <stdlib.h> //exit(1)
#include <string.h> //strdup
#include <unistd.h> //fork
// gcc 5.2.cpp -o 5.2.out
int main() {
  int fd = open("./5.2.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
  assert(fd > -1);
  // return code
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("hello, I am child (pid:%d)\n", (int)getpid());
    // print helloworld to 5.2.output
    // fd is short for file descriptor
    int rc = write(fd, "hello world!(child)", 19);
    assert(rc == 19);
  } else {
    // int rc_wait = wait(NULL);
    printf("hello, I am parent of %d (pid:%d)\n", rc, (int)getpid());
    int rc = write(fd, "hello world!(parent)", 20);
    assert(rc == 20);
  }
  return 0;
}
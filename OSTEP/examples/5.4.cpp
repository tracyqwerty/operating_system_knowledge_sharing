#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    close(STDOUT_FILENO);
    open("./5.4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    char *myargs[3];
    myargs[0] = strdup("wc");
    myargs[1] = strdup("5.1.cpp");
    myargs[2] = NULL;
    execvp(myargs[0], myargs);
  } else {
    int rc_wait = wait(NULL);
  }
  return 0;
}
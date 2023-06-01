#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// gcc 5.8.cpp -o 5.8.out
void check_fork(pid_t rc) {
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
}
int main() {
  int pipefd[2];
  pid_t pid1, pid2;
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  pid1 = fork();
  check_fork(pid1);
  if (pid1 == 0) { // child1
    printf("hello, I am child1 (pid:%d)\n", (int)getpid());
    close(pipefd[0]); // close reading end in the child
    dup2(pipefd[1], STDOUT_FILENO);
    execlp("echo", "echo", "hello from child1!", NULL);
  } else { // father
    pid2 = fork();
    check_fork(pid2);
    if (pid2 == 0) { // child2
      printf("hello, I am child2 (pid:%d)\n", (int)getpid());
      close(pipefd[1]); // close the writing end of the pipe in the child
      dup2(pipefd[0], STDIN_FILENO);
      execlp("cat", "cat", NULL);
    } else { // father
      pid_t rc_wait = wait(NULL);
      printf("hello, I am parent of %d (pid:%d)(rc_wait:%d)\n", pid1,
             (int)getpid(), rc_wait);
    }
  }
  return 0;
}
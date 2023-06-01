#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// gcc 5.4.cpp -o 5.4.out
void check_fork(pid_t rc) {
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    // terminate the process immediately
    exit(1);
  }
}
int main() {
  pid_t rc1 = fork();
  check_fork(rc1);
  // execl(), execlp(), execle(), execv(), execvp(), and execve()
  if (rc1 == 0) { // child1
    printf("hello, I am child1 (pid:%d)\n", (int)getpid());
    char *myargs[3];
    myargs[0] = strdup("/bin/ls");
    myargs[1] = strdup("-l");
    myargs[2] = NULL;
    // execv(myargs[0], myargs);
  } else { // father
    int rc_wait = wait(NULL);
    pid_t rc2 = fork();
    check_fork(rc2);
    if (rc2 == 0) { // child2
      printf("hello, I am child2 (pid:%d)\n", (int)getpid());
      char *myargs[3];
      myargs[0] = strdup("ls");
      myargs[1] = strdup("-a");
      myargs[2] = NULL;
      // execvp(myargs[0], myargs);
    } else { // father
      // int rc_wait = wait(NULL);
      pid_t rc3 = fork();
      check_fork(rc3);
      if (rc3 == 0) { // child3
        printf("hello, I am child3 (pid:%d)\n", (int)getpid());
        char *myargs[3];
        myargs[0] = strdup("/bin/ls");
        myargs[1] = strdup("-al");
        myargs[2] = NULL;

        char *envp[2];
        envp[0] = strdup("/bin/ls");
        envp[1] = NULL;
        // if (execve(myargs[0], myargs, envp) == -1) {
        //   perror("execve");
        //   exit(EXIT_FAILURE);
        // }
      } else {
        int rc_wait = wait(NULL);
        pid_t rc4 = fork();
        check_fork(rc4);
        if (rc4 == 0) { // child4
          printf("hello, I am child4 (pid:%d)\n", (int)getpid());
          // test execl():
          // the first argument is the path of the program
          // the second is the name of program. argv[0].
          // often set  to the name of the program without a path prefix.
          // however, it can be set to any arbitrary value in most cases.
          // (but we stronly recommend you not do that!)
          // execl("/bin/ls", "tracyqwerty", "-l", NULL); // This also works
          execl("/bin/ls", "ls", "-l", NULL);
        } else {
          // wait for one of the children process to terminate
          /*
          If a child process finishes before the parent process calls
          `wait()`, the child process's exit status is stored by the operating
          system until the parent retrieves it using `wait()`. This is known as
          a "zombie" process - it has finished executing, but it still has an
          entry in the process table so the parent can find out its exit status.

          In this case, when the parent eventually calls `wait()`, it won't
          actually have to wait because the child has already finished. The
          `wait()` call will return immediately with the exit status of the
          child.

          So, to answer your question, if a child has already completed
          execution before the parent calls `wait()`, the call to `wait()` will
          be immediately satisfied with the status of that child. The parent
          will not wait for another child process.

          If there are still other child processes running after this, a
          subsequent call to `wait()` would then wait for one of those remaining
          child processes to finish.
          */
          int rc_wait = wait(NULL);
          printf("hello, I am parent of %d (pid:%d)(rc_wait:%d)\n", rc1,
                 (int)getpid(), rc_wait);
        }
      }
    }
  }
  return 0;
}

/*
NAME
    execvp(exec-v-Path), execv(exec-argV) - execute a file
The execvp function is similar to execv , except that it searches the
directories listed in the PATH environment variable (see Standard Environment
Variables) to find the full file name of a file from filename if filename does
not contain a slash.

    execvpe(exec-v-p-Env)

e – An array of pointers to environment variables is explicitly passed to the
>new process image.
l – Command-line arguments are passed individually (a list) to the function.
p – Uses the PATH environment variable to find the file named in the file
>argument to be executed.
v – Command-line arguments are passed to the function as an array (vector) of
pointers.

https://www.cnblogs.com/mickole/p/3187409.html

*/
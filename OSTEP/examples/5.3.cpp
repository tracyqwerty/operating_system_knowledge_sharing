#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
// gcc 5.3.cpp -o 5.3.out
int main(int argc, char *argv[]) {
  printf("hello world (pid:%d)\n", (int)getpid());
  // rc is short for return code
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) {
    printf("hello, I am child (pid:%d)\n", (int)getpid());
    char *myargs[3];
    // the strdup() function returns a pointer to a new string which is a
    // duplicate of the string s. Memory for the new string is obtained with
    // malloc(3), and can be freed with free(3).
    myargs[0] = strdup("wc");      // program: "wc" (word count)
    myargs[1] = strdup("5.1.cpp"); // argument: file to count
    myargs[2] = NULL;              // marks end of array
    // myargs[0] is the name of the program to be executed
    // myargs is the array of arguments that will be passed to the "wc" program.
    execvp(myargs[0], myargs); // runs word count
    printf("this shouldn't print out.\n");
  } else {
    int rc_wait = wait(NULL);
    printf("hello, I am parent of %d (pid:%d)(rc_wait:%d)\n", rc, (int)getpid(),
           rc_wait);
  }
  return 0;
}
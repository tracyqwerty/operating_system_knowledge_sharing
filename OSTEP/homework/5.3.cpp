#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
// gcc 5.3.cpp -o 5.3.out
//  Signal handler for SIGUSR1
void sigusr1_handler(int signum) { printf("Goodbye\n"); }

int main() {
  // Install the signal handler
  signal(SIGUSR1, sigusr1_handler);

  // Create a child process
  pid_t pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    return 1;
  } else if (pid == 0) {
    // We're in the child process
    printf("Hello\n");
    // Send a signal to the parent process
    kill(getppid(), SIGUSR1);
  } else {
    // We're in the parent process
    // Wait for the signal before printing
    // When it receives the SIGUSR1 signal, it calls the signal handler, which
    // prints "Goodbye".

    // using pause() like this can potentially create a race condition if the
    // child process sends the signal before the parent process starts pausing.
    pause();
  }

  return 0;
}

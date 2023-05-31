#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
// gcc 5.3_robust.cpp -o 5.3_robust.out
//  Signal handler for SIGUSR1
void sigusr1_handler(int signum) {
  // Nothing needs to be done here
}

int main() {
  // Install the signal handler
  struct sigaction sa;
  sa.sa_handler = sigusr1_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGUSR1, &sa, NULL) == -1) {
    fprintf(stderr, "Failed to install signal handler\n");
    return 1;
  }

  // Block SIGUSR1
  sigset_t mask, oldmask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGUSR1);
  if (sigprocmask(SIG_BLOCK, &mask, &oldmask) < 0) {
    fprintf(stderr, "Failed to block signal\n");
    return 1;
  }

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
    while (1) {
      sigsuspend(&oldmask);
      break;
    }
    printf("Goodbye\n");

    // Restore the old signal mask
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
      fprintf(stderr, "Failed to restore signal mask\n");
      return 1;
    }
  }

  return 0;
}

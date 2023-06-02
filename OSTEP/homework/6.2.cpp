#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NUM_ITERATIONS 10000000
// gcc 6.2.cpp -o 6.2.out
int main() {
  int fds[2];
  pipe(fds);

  if (fork() == 0) {
    // Child process
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      char c;
      read(fds[0], &c, 1);  // read from parent
      write(fds[1], &c, 1); // write to parent
    }
  } else {
    // Parent process
    struct timespec start, end;

    char c = 'x';
    write(fds[1], &c, 1); // kick off the switching

    clock_gettime(CLOCK_REALTIME, &start);

    for (int i = 0; i < NUM_ITERATIONS; i++) {
      read(fds[0], &c, 1);  // read from child
      write(fds[1], &c, 1); // write to child
    }

    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate the time taken, in seconds
    double time_taken =
        (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    time_taken /= NUM_ITERATIONS * 2; // per context switch

    printf("Average time per context switch: %f seconds\n", time_taken);
  }

  return 0;
}
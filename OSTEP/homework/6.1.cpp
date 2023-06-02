#include <stdio.h>
#include <time.h>
#include <unistd.h>
// gcc 6.1.cpp -o 6.1.out
int main() {
  struct timespec start, end;
  int i;

  // Get the start time
  clock_gettime(CLOCK_REALTIME, &start);

  // Call getpid() in a loop
  for (i = 0; i < 1000000; i++) {
    getpid();
  }

  // Get the end time
  clock_gettime(CLOCK_REALTIME, &end);

  // Calculate the time taken, in seconds
  double time_taken =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

  printf("Time taken for a million getpid() calls: %f seconds\n", time_taken);

  return 0;
}

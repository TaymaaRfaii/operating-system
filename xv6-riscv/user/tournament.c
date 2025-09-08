#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: tournament <num_processes>\n");
    exit(1);
  }
  int n = atoi(argv[1]);
  int tid = tournament_create(n);
  if (tid < 0) {
    printf("Failed to create tournament tree\n");
    exit(1);
  }

  // Acquire the tournament lock (up to root)
  if (tournament_acquire() < 0) {
    printf("Process %d failed to acquire lock\n", tid);
    exit(1);
  }

  // Critical section (only one should print at a time)
  printf("Process with tournament ID %d, PID %d is in critical section\n", tid, getpid());
  
  // Release the locks (down to leaf)
  if (tournament_release() < 0) {
    printf("Process %d failed to release lock\n", tid);
    exit(1);
  }

  exit(0);
}

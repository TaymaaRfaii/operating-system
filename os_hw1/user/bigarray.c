#include "user/user.h"
#include "kernel/types.h"


#define CHILDREN 4
#define ARR_SIZE (1 << 16)

int
main(int argc, char *argv[])
{
  static int data[ARR_SIZE];
  for (int i = 0; i < ARR_SIZE; i++) {
    data[i] = i;
  }

  int pids[CHILDREN];
  int res = forkn(CHILDREN, pids);
  if (res == -1) {
    printf("forkn failed.\n");
    exit(1, "");
  }

  if (res > 0) {
    int segment = ARR_SIZE / CHILDREN;
    int start = (res - 1) * segment;
    int end = start + segment;
    long part_sum = 0;
    for (int i = start; i < end; i++) {
      part_sum += data[i];
    }
    exit((int)part_sum, "");
  } else {
    printf("Parent process: created children\n");
    for (int i = 0; i < CHILDREN; i++) {
      printf("Child %d pid = %d\n", i + 1, pids[i]);
    }

    int finished;
    int results[64];
    if (waitall(&finished, results) < 0) {
      printf("waitall failed.\n");
      exit(1, "");
    }

    if (finished != CHILDREN) {
      printf("Error: expected %d children, got %d\n", CHILDREN, finished);
      exit(1, "");
    }

    long total = 0;
    for (int i = 0; i < CHILDREN; i++) {
      printf("Child %d returned %d\n", i + 1, results[i]);
      total += results[i];
    }
    printf("Final Sum = %d\n", total);
    exit(0, "Done");
  }
}

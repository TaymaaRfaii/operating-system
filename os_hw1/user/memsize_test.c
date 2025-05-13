#include "kernel/types.h"
#include "user/user.h"

int main() {
  printf("Initial memsize: %d bytes\n", memsize());

  void *block = malloc(20 * 1024); // 20 KB
  if (!block) {
    printf("Memory allocation failed!\n");
    exit(1, "");
  }

  printf("After malloc(20KB): %d bytes\n", memsize());

  free(block);

  printf("After free: %d bytes\n", memsize());

  exit(0, "");
}

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Global variables
int process_id;      
int tournament_levels = 0;   
int locks_total_num = 0;      
int *locks = 0;           // Array of Peterson lock IDs

// helper functions
int is_power_of_2(int x) {

  return x > 0 && (x & (x - 1)) == 0;
}

int calculate_levels(int n) {
  int levels = 0;
  while (n > 1) {
    n >>= 1;
    levels++;
  }
  return levels;
}

int allocate_peterson_locks(int num_locks) {
  locks = malloc(sizeof(int) * num_locks);
  if (!locks) return -1;

  for (int i = 0; i < num_locks; i++) {
    locks[i] = peterson_create();
    if (locks[i] < 0) return -1;
  }
  return 0;
}
//__________________________________________________

int tournament_create(int processes) {
  if (!is_power_of_2(processes) || processes > 16)
    return -1;

  tournament_levels = calculate_levels(processes);
  locks_total_num = processes - 1;

  if (allocate_peterson_locks(locks_total_num) < 0)
    return -1;

  for (int i = 1; i < processes; i++) {
    int pid = fork();
    if (pid < 0) {
      return -1;
    }
    if (pid == 0) {
      process_id = i;
      return process_id;
    }
  }

  // Parent process
  process_id = 0;
  return process_id;
}
//____________________________________________________________

int tournament_acquire(void) {
  if (process_id < 0 || !locks || tournament_levels <= 0)
    return -1;

  for (int level = tournament_levels - 1; level >= 0; level--) {
    // Calculate the role (0 = left, 1 = right) at this level
    int bit = tournament_levels - level - 1;
    int role = (process_id & (1 << bit)) >> bit;


    // Calculate lock index in BFS layout
    int node = process_id >> (tournament_levels - level);
    int lock_index = node + (1 << level) - 1;

    if (peterson_acquire(locks[lock_index], role) < 0) {
      return -1;
    }
  }

  return 0;
}
//__________________________________________________________________________

int tournament_release(void) {
  if (process_id < 0 || !locks || tournament_levels <= 0)
    return -1;

  for (int level = 0; level < tournament_levels; level++) {
    // Determine role at this level
    int bit = tournament_levels - level - 1;
    int role = (process_id & (1 <<bit )) >> bit;

    // Compute lock index in BFS order
    int node = process_id >> (tournament_levels - level);
    int lock_index = node + ((1 << level) - 1);

    if (peterson_release(locks[lock_index], role) < 0)
      return -1;
  }

  return 0;
}

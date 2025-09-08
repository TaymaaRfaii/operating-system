#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

#include "Petersonlock.h"

static struct petersonlock p_locks[MAXPETERSON];

void
peterson_init(void) {
  for (int i = 0; i < MAXPETERSON; i++) {
    p_locks[i].lockid = i;
    p_locks[i].active = 0;
    p_locks[i].flag[0] = 0;
    p_locks[i].flag[1] = 0;
    p_locks[i].turn = 0;
  }
}

int
peterson_create(void) {
  for (int id = 0; id < MAXPETERSON; id++) {
    if (__sync_lock_test_and_set(&p_locks[id].active, 1) == 0) {
      __sync_synchronize(); // full memory barrier
      p_locks[id].flag[0] = 0;
      p_locks[id].flag[1] = 0;
      p_locks[id].turn = 0;
      return id;
    }
  }
  return -1; // all locks used
}

int
peterson_acquire(int lock_id, int role) {
  if (lock_id < 0 || lock_id >= MAXPETERSON || role < 0 || role > 1)
    return -1;

  struct petersonlock *p = &p_locks[lock_id];

  if (p->active == 0)
    return -1;

  __sync_lock_test_and_set(&p->flag[role], 1);
  __sync_synchronize();
  p->turn = 1 - role;
  __sync_synchronize();

  while (__sync_synchronize(), p->flag[1 - role] && p->turn == 1 - role)
    yield();

  return 0;
}

int
peterson_release(int lock_id, int role) {
  if (lock_id < 0 || lock_id >= MAXPETERSON || role < 0 || role > 1)
    return -1;

  struct petersonlock *p = &p_locks[lock_id];

  if (p->active == 0)
    return -1;

  __sync_lock_release(&p->flag[role]);
  __sync_synchronize();
  return 0;
}

int
peterson_destroy(int lock_id) {
  if (lock_id < 0 || lock_id >= MAXPETERSON)
    return -1;

  struct petersonlock *p = &p_locks[lock_id];

  if (p->active == 0)
    return -1;

  p->flag[0] = 0;
  p->flag[1] = 0;
  p->turn = 0;
  __sync_synchronize();
  __sync_lock_release(&p->active);

  return 0;
}

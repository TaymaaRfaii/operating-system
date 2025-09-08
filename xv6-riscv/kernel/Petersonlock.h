#ifndef PETERSONLOCK_H
#define PETERSONLOCK_H

#define MAXPETERSON 15

struct petersonlock {
  volatile int flag[2]; // MUST be volatile for synchronization
  volatile int turn;
  int active;           // 1 = in use
  int lockid;
};

void peterson_init(void);
int peterson_create(void);
int peterson_acquire(int lock_id, int role);
int peterson_release(int lock_id, int role);
int peterson_destroy(int lock_id);

#endif

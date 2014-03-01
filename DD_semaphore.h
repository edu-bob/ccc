#include <parallel/parallel.h>

typedef struct pidTag {
	int pid;
	struct pidTag *next;
} waiting;


typedef struct {
	int count;
	slock_t *lock;
	waiting *head;
	waiting *tail;
} Semaphore;
typedef Semaphore *semaphore;
void DD_sem_ask(), DD_sem_tell(), DD_sem_tell_num();
semaphore DD_sem_create();


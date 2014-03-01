/*
 *  Declarations for the bits of the Concurrent C kernel that are visible
 *  in the generated C code.
 */

typedef char *CC_tblock;
typedef int CC_void;
typedef struct CC_pid_struct *CC_pid;
typedef struct CC_task_struct *CC_task;
typedef char CC_bool;

#define c_nullpid ((CC_pid)0)

extern float CC_bestdelay,CC_trydelay;
extern int CC_achoice,CC_dchoice,CC_ichoice,CC_ochoice;
extern CC_bool CC_terminate_exists,CC_accept_exists;

extern CC_pid CC_mypid,CC_childpid;
extern CC_task CC_me;
extern CC_tblock CC_mytblock;

char *CC_shmalloc();

/*
 *  note that CC_delay() should include a cast to double...
 */

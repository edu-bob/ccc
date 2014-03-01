#include <ccc/CC_kernel.h>
main (argc,argv, envp)
int argc;
char *argv[],**envp;
{

	CC_main(argc, argv, envp);
	
}

CC_fork() {}
CC_accept() {}
CC_treturn() {}
CC_complete() {}
CC_delay() {}
CC_tcallinit() {}
CC_tcall() {}
	
	


float CC_bestdelay,CC_trydelay;
int CC_achoice,CC_dchoice,CC_ichoice,CC_ochoice;
int CC_terminate_exists,CC_accept_exists;

CC_pid CC_mypid,CC_childpid;
CC_task CC_me;
CC_tblock CC_mytblock;


/*---------------------------------------------------------------------------
 * The following definitions are all used by the debugging system.
 * 
 * William L. Lynch 		Mon Jun 30 16:39:20 1986
 *---------------------------------------------------------------------------
 */

#ifdef NORMAL		/* for non-debugging compilation, with blazing speed */
#define CC_block(event)		sigpause(0)
#define CC_unblock(t, event)	kill((t)->unixpid,SIGUNBLOCK)
#define CC_kill(t)		kill((t)->unixpid,SIGTERMINATE)
#endif

				/* {b,u,k} means block, unblock, or kill
				 is happening */
typedef enum {
	DD_NONE,		/* b CC_tcall, actual blocking for trans */
	DD_WAITbORN,		/* b CC_fork, parent waiting for childbirth */
	DD_ISbORN,		/* u CC_fork, child signaling being born */
	DD_WAITaBORT,		/* b CC_checkout, wait to be aborted */
				/* b also in CC_complete */
	DD_SELECTtERM,		/* b CC_terminateifokay, chose termination
				 option, waiting to die ? , also appears in
				 CC_preparedtoterm */
	DD_LASTuSERkILL,	/* k CC_checkin, aborted on last loan */
	DD_CHILDkILL,		/* k CC_children_select_terminate, killing
				 all children */
				/* k also appears in CC_abort */
	DD_WAITcHILDdIE,	/* b CC_selfabort, waiting for the last
				 child to signal his death */
				/* also in c_wait */
				/* also appears in CC_complete */
	DD_LASTcHILD,		/* u CC_selfabort, the last child to die
				 is signalling */
	DD_EXITING,		/* exit at end of selfabort */
	DD_IMdYING,		/* u CC_flushclients, clients are waiting,
				 but I'm going to die anyway */
				/* also in CC_flushqueues, with two sent
				 for waiting processes */
	DD_ABORTINGyOU,		/* k c_abort, an abort is being sent */
	DD_EATtIMER,		/* b CC_flush_DELAY, eats up a timer
				 signal which was preceded by another signal */
	DD_EATtRANS,		/* b CC_flush_ACCEPT, eats up a
				 transaction signal which arrived too late */
	DD_EATtERM,		/* b CC_flush_TERMINATE, eat a terminate
				 signal which arrived too late ? */
	DD_IMMEDsERV,		/* u CC_timedcall, you were waiting when I
				 called */
	DD_WAITtRANS,		/* b CC_timedcall, I will wait for your
				 immediate service (see above), or not
				 available for service, I'll wait */
				/* also in CC_tcall_sleep, after a delay
				 snuck in too late */
	DD_WAITtIME,		/* b CC_tcall_sleep, waiting for a
				 possible timeout on a timed transaction
				 call */
	DD_TIMEOUT,		/*   CC_tcall_sleep, a timed tcall failed */
	DD_WAITaCCEPT,		/* b CC_accept, no one is signalling a
				 transaction */
	DD_WAITsELECT,		/* b CC_select, nothing available in a
				 select */
	DD_DELAY,		/* b CC_delay, a delay statement was
				 encountered */
	DD_ACCEPT,		/* u CC_accept_postlude, accepting a
				 client */
	DD_TRETURN,		/* u CC_treturn, doing a treturn */
	DD_TRETURNED,		/*   same as WAITtRANS, I got a return
				 after waiting */
	DD_COMMUN		/* communication is ending */
} DD_eventType;

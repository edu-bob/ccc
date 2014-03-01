#include <sys/time.h>

typedef struct buf {
	DD_eventType event;	/* kind of event */
	int	id;		/* unique event id */
	int	fromId;		/* source unix pid */
	int	toId;		/* dest unix pid, conversion pointer if no dest,
							and taskp on waitTrans */
	char	*tblock;	/* shared memory tblock pointer,
					 and ccc pid (task pointer) on creates */
	void	(*convert)();	/* conversion function for tblock */
	struct	timeval time;	/* time stamp */
	int	bufNum;
} buffer;


void DD_buffers_addEmpty(), DD_buffers_addFull();
buffer *DD_buffers_getEmpty(), *DD_buffers_getFull();
char *DD_buffers_getTblock();

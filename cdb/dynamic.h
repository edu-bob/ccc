#include "points.h"

typedef enum {
	running,
	accepting,
	dead,
	waitBorn,
	waitAbort,
	selectTerm,
	waitChildDie,
	eating,
	waitTrans,
	waitTime,
	waitAccept,
	waitSelect,
	delay,
	timeOut,
	dying,
	treturned
} state;

typedef struct {
	int row, col;		/* screen position */
	int channel;
	unsigned long show;	/* what to display */
} displayInfo;


typedef struct processTag {
	string name;
	string tag;		/* cdb tag */
	unsigned tagId;		/* cdb creation number, starts at 0 per spec */
	unsigned pid;		/* unix pid */
	unsigned taskp;		/* ccc task pointer */
	state state;		/* current process state */
	unsigned createId;	/* id number of create occur */
	unsigned id;		/* id of occur causing current state */
	char *transactName;	/* used for treturn printout */
	struct processTag *with;/* process in communication */
	unsigned numParms;	/* number of create, accept, or return parms */
	unsigned spaceParms;	/* symbol name space of above */
	string *parms;		/* for create, accept, and treturn */
	unsigned long time;	/* time of last event */
	symbol *symbol;		/* process symbol entry */
	staticProcessInfo *info;/* occurs */
	displayInfo  display;	/* where and what to display about this process */
	pointsType point;	/* for break and trace points */
} process;


typedef struct listTag {
	process *process;
	struct listTag *next;
} processList;
	

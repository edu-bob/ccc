typedef DD_ProcessTable  pointsInfo;		/* per process spec */

typedef struct {
	string name;
	unsigned tagId;		/* used to assign <n> to each process */
	pointsInfo points;	/* number of occurrences per process */
} staticProcessInfo;

staticProcessInfo *static_infoLookup();



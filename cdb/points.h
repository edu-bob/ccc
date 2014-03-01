typedef struct {
	unsigned short traces:1;
	unsigned short breaks:1;
} pointType;


typedef struct {
	pointType *accept, *create, *transact, *delay, 
						*terminate, *select, *treturn;
} pointsType;


typedef int point;
typedef point *idToPoint;


#define ON 1
#define OFF 0
#define TRACE 1
#define BREAK 0

bool points_accept(), points_create(), points_transact(), points_select(), 
	points_delay(), points_terminate(), points_treturn();

#include <ccc/CC_kernel.h>

# line 1 makes-time-error.cc

# line 1 /usr/include/math.h

extern double fabs (),floor ()
# line 16 /usr/include/math.h
,ceil (),fmod ()
# line 16 /usr/include/math.h
,ldexp (),frexp ()
# line 16 /usr/include/math.h
;
extern double sqrt (),hypot ()
# line 17 /usr/include/math.h
,atof ();
extern double sin ()
# line 18 /usr/include/math.h
,cos (),tan ()
# line 18 /usr/include/math.h
,asin (),acos ()
# line 18 /usr/include/math.h
,atan (),atan2 ()
# line 18 /usr/include/math.h
;
extern double exp (),log ()
# line 19 /usr/include/math.h
,log10 (),pow ()
# line 19 /usr/include/math.h
;
extern double sinh (),cosh ()
# line 20 /usr/include/math.h
,tanh ();
extern double gamma ()
# line 21 /usr/include/math.h
;
extern double j0 (),j1 ()
# line 22 /usr/include/math.h
,jn (),y0 ()
# line 22 /usr/include/math.h
,y1 (),yn ()
# line 22 /usr/include/math.h
;

# line 53 makes-time-error.cc

typedef float Real ;
typedef Real **RealMatrix 
# line 55 makes-time-error.cc
;

/* PROCESS SPEC Protected_Counter */
CC_pid CC_Protected_Counter();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 64 makes-time-error.cc
	    value;
	int arg1;
} 
CC_8Initialize;
#define CC_8Initialize_id 0

# line 64 makes-time-error.cc

typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 
# line 65 makes-time-error.cc
	    value;
	int arg1;
} 
CC_8Incr;
#define CC_8Incr_id 1

# line 65 makes-time-error.cc

typedef int CC_headvalue3;
typedef struct {
	CC_headvalue3 
# line 66 makes-time-error.cc
	    value;
	int arg1;
} 
CC_8Decr;
#define CC_8Decr_id 2

# line 66 makes-time-error.cc

typedef int CC_headvalue4;
typedef struct {
	CC_headvalue4 
# line 67 makes-time-error.cc
	    value;
} 
CC_8Read;
#define CC_8Read_id 3

typedef int CC_headvalue5;

# line 68 makes-time-error.cc
typedef struct {
	CC_headvalue5 value;
} 
CC_8WakeWhenZero;
#define CC_8WakeWhenZero_id 4

# line 68 makes-time-error.cc

#define CC_Protected_Counter_numtrans 5

# line 69 makes-time-error.cc

/* PROCESS SPEC Region_Task */
CC_pid CC_Region_Task();

typedef int CC_headvalue6;
typedef struct {
	CC_headvalue6 
# line 73 makes-time-error.cc
	    value;
	int arg1;
	int arg2;

# line 73 makes-time-error.cc
} 
CC_14SetParameter;
#define CC_14SetParameter_id 0

#define CC_Region_Task_numtrans 1

# line 74 makes-time-error.cc

/* PROCESS SPEC Coordinator_Task */
CC_pid CC_Coordinator_Task();

typedef int CC_headvalue7;

# line 78 makes-time-error.cc
typedef struct {
	CC_headvalue7 value;
} 
CC_16Wait;
#define CC_16Wait_id 0

# line 78 makes-time-error.cc

typedef int CC_headvalue8;
typedef struct {
	CC_headvalue8 
# line 79 makes-time-error.cc
	    value;
} 
CC_16KeepOnGoing;
#define CC_16KeepOnGoing_id 1

typedef int CC_headvalue9;

# line 80 makes-time-error.cc
typedef struct {
	CC_headvalue9 value;
} 
CC_16Finish;
#define CC_16Finish_id 2

# line 80 makes-time-error.cc

#define CC_Coordinator_Task_numtrans 3

# line 81 makes-time-error.cc

CC_pid Regions [50
# line 84 makes-time-error.cc
][50
# line 84 makes-time-error.cc
];
CC_pid Coordinators [
# line 85 makes-time-error.cc
50][
# line 85 makes-time-error.cc
50]
# line 85 makes-time-error.cc
;
CC_pid Unfinished_Counter ;
int RowRegions 
# line 87 makes-time-error.cc
,ColRegions ;
int NumRegions 
# line 88 makes-time-error.cc
;
int RowsPerRegion ,ColsPerRegion 
# line 89 makes-time-error.cc
;

RealMatrix U ;
typedef int  CC_shtypedef0;
CC_shtypedef0 
# line 92 makes-time-error.cc
(*DidNotConverge);
Real MaxErr 
# line 93 makes-time-error.cc
,Omega ;
int MaxIterations 
# line 94 makes-time-error.cc
,NumberOfRegions ;
int RowHi 
# line 95 makes-time-error.cc
,ColHi ;

Parallel_Relaxation (p_U ,p_MaxErr ,p_MaxIterations ,p_NumberOfRegions ,
p_DidNotConverge ,p_Omega ,p_RowHi ,p_ColHi )

# line 99 makes-time-error.cc
RealMatrix p_U ;
Real p_MaxErr 
# line 100 makes-time-error.cc
,p_Omega ;
int p_MaxIterations 
# line 101 makes-time-error.cc
,p_NumberOfRegions ,*p_DidNotConverge 
# line 101 makes-time-error.cc
;
int p_RowHi ,p_ColHi 
# line 102 makes-time-error.cc
;
{

	int I ,J 
# line 123 makes-time-error.cc
	    ;

	U =p_U 
# line 128 makes-time-error.cc
	    ;
	MaxErr =p_MaxErr 
# line 129 makes-time-error.cc
	    ;
	Omega =p_Omega 
# line 130 makes-time-error.cc
	    ;
	MaxIterations =p_MaxIterations 
# line 131 makes-time-error.cc
	    ;
	NumberOfRegions =p_NumberOfRegions 
# line 132 makes-time-error.cc
	    ;
	RowHi =p_RowHi 
# line 133 makes-time-error.cc
	    ;
	ColHi =p_ColHi 
# line 134 makes-time-error.cc
	    ;

	(*DidNotConverge) =0
# line 138 makes-time-error.cc
	    ;

	if (RowHi <2||ColHi <2
# line 143 makes-time-error.cc
	    )
		return ;

	for (
# line 148 makes-time-error.cc
I =1;
# line 148 makes-time-error.cc
I <RowHi ;
# line 148 makes-time-error.cc
I ++)
		    for (
# line 149 makes-time-error.cc
J =1;
# line 149 makes-time-error.cc
J <ColHi ;
# line 149 makes-time-error.cc
J ++)

# line 150 makes-time-error.cc
			U [I ][J ]=0.0;

# line 162 makes-time-error.cc
	NumberOfRegions =((NumberOfRegions )<((RowHi -1)*(ColHi -1))?(NumberOfRegions ):((RowHi -1)*(ColHi -1)));

# line 163 makes-time-error.cc
	RowRegions =((int)floor (sqrt (NumberOfRegions *(RowHi -1)/(Real)(ColHi -1))));

# line 164 makes-time-error.cc
	ColRegions =NumberOfRegions /RowRegions ;

# line 165 makes-time-error.cc
	NumRegions =RowRegions *ColRegions ;

# line 166 makes-time-error.cc
	printf ("R,C,NumRegions = %d, %d, %d.\n",RowRegions ,ColRegions ,NumRegions );

# line 177 makes-time-error.cc
	RowsPerRegion =((RowHi -1)+(RowRegions -1))/RowRegions ;

# line 178 makes-time-error.cc
	ColsPerRegion =((ColHi -1)+(ColRegions -1))/ColRegions ;

# line 190 makes-time-error.cc
	Unfinished_Counter =CC_Protected_Counter (0, 1 );

# line 191 makes-time-error.cc
	(CC_tcallinit(Unfinished_Counter ,CC_8Initialize_id),
	((CC_8Initialize *)CC_mytblock)->arg1= NumRegions ,
	CC_tcall(1),
	((CC_8Initialize *)CC_mytblock)->value) ;

	for (
# line 196 makes-time-error.cc
I =1;
# line 196 makes-time-error.cc
I <=RowRegions ;
# line 196 makes-time-error.cc
I ++)
		    for (
# line 197 makes-time-error.cc
J =1;
# line 197 makes-time-error.cc
J <=ColRegions ;
# line 197 makes-time-error.cc
J ++){

# line 198 makes-time-error.cc
			Regions [I ][J ]=CC_Region_Task (I ,J ,0, 2 );

# line 199 makes-time-error.cc
			Coordinators [I ][J ]=CC_Coordinator_Task (0, 3 );
		}

	for (
# line 204 makes-time-error.cc
I =1;
# line 204 makes-time-error.cc
I <=RowRegions ;
# line 204 makes-time-error.cc
I ++)
		    for (
# line 205 makes-time-error.cc
J =1;
# line 205 makes-time-error.cc
J <=ColRegions ;
# line 205 makes-time-error.cc
J ++)

# line 206 makes-time-error.cc
			(CC_tcallinit(Regions [I ][J ],CC_14SetParameter_id),
			((CC_14SetParameter *)CC_mytblock)->arg1= I ,
			((CC_14SetParameter *)CC_mytblock)->arg2= J ,
			CC_tcall(2),
			((CC_14SetParameter *)CC_mytblock)->value) ;

# line 211 makes-time-error.cc
	(CC_tcallinit(Unfinished_Counter ,CC_8WakeWhenZero_id),
	CC_tcall(3),
	((CC_8WakeWhenZero *)CC_mytblock)->value) ;

# line 212 makes-time-error.cc
	*p_DidNotConverge =(*DidNotConverge) ;
}

/* PROCESS BODY Protected_Counter */
CC_pid CC_Protected_Counter(
CC_priority, DD_createId)

# line 221 makes-time-error.cc
int CC_priority;
unsigned DD_createId;
{

	int Counter 
# line 222 makes-time-error.cc
	    ;

	if (CC_fork("Protected_Counter",CC_Protected_Counter_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 224 makes-time-error.cc
	{
		CC_tblock CC_tblock0;

# line 224 makes-time-error.cc

# line 224 makes-time-error.cc
		CC_accept(CC_8Initialize_id,&CC_tblock0,0,0, 1);
		{

# line 225 makes-time-error.cc
			Counter =((CC_8Initialize *)CC_tblock0)->arg1 ;
		}
CC_treturn0: 
		CC_treturn(1);
	}

# line 226 makes-time-error.cc

	for (;;)
	{
		CC_selectinit();
		goto CC_guard1_1; 
CC_choose1: 
		switch(CC_choose(1)) {
# line 228 makes-time-error.cc
CC_guard1_1 :
# line 228 makes-time-error.cc

			if ((CC_accept_exists=1,CC_tryaccept(CC_8Incr_id,1)))
				goto CC_choose1;
			goto CC_guard1_2;
		case 1:
# line 229 makes-time-error.cc
			{
				CC_tblock CC_tblock1;

# line 229 makes-time-error.cc

# line 229 makes-time-error.cc
				CC_accept(CC_8Incr_id,&CC_tblock1,0,1, 2);
				{

# line 230 makes-time-error.cc
					Counter =Counter +((CC_8Incr *)CC_tblock1)->arg1 ;

# line 231 makes-time-error.cc
					printf ("after Incr, Counter is now %d.\n",Counter );
				}
CC_treturn1: 
				CC_treturn(2);
			}

# line 232 makes-time-error.cc

			break; 
# line 233 makes-time-error.cc
CC_guard1_2 :
			if ((CC_accept_exists=1,CC_tryaccept(CC_8Decr_id,2)))
				goto CC_choose1;
			goto CC_guard1_3;
		case 2:
# line 234 makes-time-error.cc
			{
				CC_tblock CC_tblock2;

# line 234 makes-time-error.cc

# line 234 makes-time-error.cc
				CC_accept(CC_8Decr_id,&CC_tblock2,0,1, 3);

# line 234 makes-time-error.cc
				{
					if (Counter !=0
# line 235 makes-time-error.cc
					    )
						Counter =Counter -((CC_8Decr *)CC_tblock2)->arg1 
# line 236 makes-time-error.cc
						    ;
					printf ("after Decr, Counter is now %d.\n",Counter )
# line 237 makes-time-error.cc
						;
				}
CC_treturn2: 
				CC_treturn(3);
			}

# line 238 makes-time-error.cc

			break; 
CC_guard1_3 :
# line 239 makes-time-error.cc

			if ((CC_accept_exists=1,CC_tryaccept(CC_8Read_id,3)))
				goto CC_choose1;
			goto CC_guard1_4;
		case 3:
# line 240 makes-time-error.cc
			{
				CC_tblock CC_tblock3;

# line 240 makes-time-error.cc
				CC_accept(CC_8Read_id,&CC_tblock3,0,1, 4);

# line 240 makes-time-error.cc
				{
					{
						((CC_8Read *)CC_tblock3)->value = Counter 
# line 241 makes-time-error.cc
						    ;
						goto CC_treturn3;
					}

				}
CC_treturn3: 
				CC_treturn(4);
			}

# line 242 makes-time-error.cc

			break; 
# line 243 makes-time-error.cc
CC_guard1_4 :
			if ((
# line 243 makes-time-error.cc
			Counter ==0) && 
			    (CC_accept_exists=1,CC_tryaccept(CC_8WakeWhenZero_id,4)))
				goto CC_choose1;
			goto CC_guard1_5;
		case 4:
# line 244 makes-time-error.cc
			{
				CC_tblock CC_tblock4;

# line 244 makes-time-error.cc

# line 244 makes-time-error.cc
				CC_accept(CC_8WakeWhenZero_id,&CC_tblock4,0,1, 5);
CC_treturn4: 
				CC_treturn(5);
			}

# line 244 makes-time-error.cc

			break; 
# line 245 makes-time-error.cc
CC_guard1_5 :
			if (1) CC_terminate_exists=1; 
			goto CC_guard1_6;

# line 246 makes-time-error.cc
			break; 
# line 246 makes-time-error.cc

		}
		goto CC_endselect1;
CC_guard1_6: 
		goto CC_choose1;
CC_endselect1:;
	}

# line 247 makes-time-error.cc

	CC_complete();
}

/* PROCESS BODY Coordinator_Task */
CC_pid CC_Coordinator_Task(
CC_priority, DD_createId)
int CC_priority;
unsigned DD_createId;
{

# line 251 makes-time-error.cc

	int Had_KeepOnGoing =
# line 252 makes-time-error.cc
	    0;
	int Had_Finish 
# line 253 makes-time-error.cc
	    =0
# line 253 makes-time-error.cc
	    ;

	if (CC_fork("Coordinator_Task",CC_Coordinator_Task_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 255 makes-time-error.cc
	for (;;)
	{
		CC_selectinit();
		goto CC_guard2_1; 
CC_choose2: 
		switch(CC_choose(2)) {
# line 256 makes-time-error.cc
CC_guard2_1 :
			if ((CC_accept_exists=1,CC_tryaccept(CC_16KeepOnGoing_id,1)))
				goto CC_choose2;
			goto CC_guard2_2;
		case 1:
# line 257 makes-time-error.cc
			{
				CC_tblock CC_tblock5;

# line 257 makes-time-error.cc

# line 257 makes-time-error.cc
				CC_accept(CC_16KeepOnGoing_id,&CC_tblock5,0,1, 6);
				{
					if (
# line 258 makes-time-error.cc
					!Had_KeepOnGoing ){

# line 259 makes-time-error.cc
						(CC_tcallinit(Unfinished_Counter ,CC_8Incr_id),
						((CC_8Incr *)CC_mytblock)->arg1= 1,
						CC_tcall(4),
						((CC_8Incr *)CC_mytblock)->value) ;

# line 260 makes-time-error.cc
						Had_KeepOnGoing =1;
					}
				}
CC_treturn5: 
				CC_treturn(6);
			}

# line 262 makes-time-error.cc

			break; 
# line 263 makes-time-error.cc
CC_guard2_2 :
			if ((
# line 263 makes-time-error.cc
			Had_KeepOnGoing ||Had_Finish ) && 
			    (CC_accept_exists=1,CC_tryaccept(CC_16Wait_id,2)))
				goto CC_choose2;
			goto CC_guard2_3;
		case 2:
# line 264 makes-time-error.cc
			{
				CC_tblock CC_tblock6;

# line 264 makes-time-error.cc

# line 264 makes-time-error.cc
				CC_accept(CC_16Wait_id,&CC_tblock6,0,1, 7);
				{

# line 265 makes-time-error.cc
					Had_KeepOnGoing =0;
				}
CC_treturn6: 
				CC_treturn(7);
			}

# line 266 makes-time-error.cc

			break; 
# line 267 makes-time-error.cc
CC_guard2_3 :
			if ((CC_accept_exists=1,CC_tryaccept(CC_16Finish_id,3)))
				goto CC_choose2;
			goto CC_guard2_4;
		case 3:
# line 268 makes-time-error.cc
			{
				CC_tblock CC_tblock7;

# line 268 makes-time-error.cc

# line 268 makes-time-error.cc
				CC_accept(CC_16Finish_id,&CC_tblock7,0,1, 8);
				{

# line 269 makes-time-error.cc
					Had_Finish =1;
				}
CC_treturn7: 
				CC_treturn(8);
			}

# line 270 makes-time-error.cc

			break; 
# line 271 makes-time-error.cc
CC_guard2_4 :
			if (1) CC_terminate_exists=1; 
			goto CC_guard2_5;

# line 272 makes-time-error.cc
			break; 
# line 272 makes-time-error.cc

		}
		goto CC_endselect2;
CC_guard2_5: 
		goto CC_choose2;
CC_endselect2:;
	}

# line 273 makes-time-error.cc

	CC_complete();
}

All_Finish ()
{
	int Rreg 
# line 282 makes-time-error.cc
	    ,Creg ;

	for (
# line 284 makes-time-error.cc
Rreg =1;
# line 284 makes-time-error.cc
Rreg <=RowRegions ;
# line 284 makes-time-error.cc
Rreg ++)
		    for (
# line 285 makes-time-error.cc
Creg =1;
# line 285 makes-time-error.cc
Creg <=ColRegions ;
# line 285 makes-time-error.cc
Creg ++)

# line 286 makes-time-error.cc
			(CC_tcallinit(Coordinators [Rreg ][Creg ],CC_16Finish_id),
			CC_tcall(5),
			((CC_16Finish *)CC_mytblock)->value) ;
}

/* PROCESS BODY Region_Task */
CC_pid CC_Region_Task(II ,JJ ,
CC_priority, DD_createId)

# line 290 makes-time-error.cc
int  II;
int  JJ;
int CC_priority;
unsigned DD_createId;
{

	int MyRowRegion 
# line 291 makes-time-error.cc
	    ,MyColRegion ;
	int MyDone 
# line 292 makes-time-error.cc
	    ,CurCount ;
	Real New_Value 
# line 293 makes-time-error.cc
	    ;
	int MyRowLow ,MyRowHi 
# line 294 makes-time-error.cc
	    ,MyColLow ,MyColHi 
# line 294 makes-time-error.cc
	    ;
	int Iters ,I 
# line 295 makes-time-error.cc
	    ,J ;

	if (CC_fork("Region_Task",CC_Region_Task_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 300 makes-time-error.cc
	{
		CC_tblock CC_tblock8;

# line 300 makes-time-error.cc

# line 300 makes-time-error.cc

# line 300 makes-time-error.cc
		CC_accept(CC_14SetParameter_id,&CC_tblock8,0,0, 9);
		{

# line 301 makes-time-error.cc
			MyRowRegion =((CC_14SetParameter *)CC_tblock8)->arg1 ;

# line 302 makes-time-error.cc
			MyColRegion =((CC_14SetParameter *)CC_tblock8)->arg2 ;
		}
CC_treturn8: 
		CC_treturn(9);
	}

# line 303 makes-time-error.cc

# line 309 makes-time-error.cc
	MyRowLow =(MyRowRegion -1)*RowsPerRegion +1;

# line 310 makes-time-error.cc
	MyColLow =(MyColRegion -1)*ColsPerRegion +1;

# line 315 makes-time-error.cc
	MyRowHi =((RowHi -1)<(MyRowLow +RowsPerRegion -1)?(RowHi -1):(MyRowLow +RowsPerRegion -1));

# line 316 makes-time-error.cc
	MyColHi =((ColHi -1)<(MyColLow +ColsPerRegion -1)?(ColHi -1):(MyColLow +ColsPerRegion -1));

# line 317 makes-time-error.cc

	printf ("[%d][%d] MyRowLow=%d MyRowHi=%d MyColLow=%d MyColHi=%d.\n",II ,JJ ,MyRowLow ,MyRowHi ,MyColLow ,MyColHi );
	for (
# line 319 makes-time-error.cc
Iters =1;
# line 319 makes-time-error.cc
Iters <=MaxIterations ;
# line 319 makes-time-error.cc
Iters ++){

# line 320 makes-time-error.cc
		MyDone =1;

		for (
# line 324 makes-time-error.cc
I =MyRowLow ;
# line 324 makes-time-error.cc
I <=MyRowHi ;
# line 324 makes-time-error.cc
I ++){
			    for (
# line 325 makes-time-error.cc
J =MyColLow ;
# line 325 makes-time-error.cc
J <=MyColHi ;
# line 325 makes-time-error.cc
J ++){

# line 326 makes-time-error.cc

				New_Value =U [I ][J ]+Omega *0.25*(U [I -1][J ]+U [I ][J -1]+U [I +1][J ]+U [I ][J +1]-4.0*U [I ][J ]);

# line 329 makes-time-error.cc

				printf ("Abs(New_Value - U[%d][%d])=%f, New_Value=%f, U[%d][%d]=%f.\n",I ,J ,((New_Value -U [I ][J ])<0.0? -(New_Value -U [I ][J ]):(New_Value -U [I ][J ])),New_Value ,I ,J ,U [I ][J ]);

				if (
# line 332 makes-time-error.cc
				((New_Value -U [I ][J ])<0.0? -(New_Value -U [I ][J ]):(New_Value -U [I ][J ]))>=MaxErr )

# line 333 makes-time-error.cc
					MyDone =0;

# line 334 makes-time-error.cc
				U [I ][J ]=New_Value ;

# line 335 makes-time-error.cc
				New_Value =U [I ][J ];

			}
		}
		if (
# line 343 makes-time-error.cc
		!MyDone ){

# line 344 makes-time-error.cc
			printf ("[%d][%d]MyDone must be false, and it is %d.\n",II ,JJ ,MyDone );
			if (
# line 345 makes-time-error.cc
			MyRowRegion !=1)

# line 346 makes-time-error.cc
				(CC_tcallinit(Coordinators [MyRowRegion -1][MyColRegion ],CC_16KeepOnGoing_id),
				CC_tcall(6),
				((CC_16KeepOnGoing *)CC_mytblock)->value) ;

			if (
# line 348 makes-time-error.cc
			MyRowRegion !=RowRegions )

# line 349 makes-time-error.cc
				(CC_tcallinit(Coordinators [MyRowRegion +1][MyColRegion ],CC_16KeepOnGoing_id),
				CC_tcall(7),
				((CC_16KeepOnGoing *)CC_mytblock)->value) ;

			if (
# line 351 makes-time-error.cc
			MyColRegion !=1)

# line 352 makes-time-error.cc
				(CC_tcallinit(Coordinators [MyRowRegion ][MyColRegion -1],CC_16KeepOnGoing_id),
				CC_tcall(8),
				((CC_16KeepOnGoing *)CC_mytblock)->value) ;

			if (
# line 354 makes-time-error.cc
			MyColRegion !=ColRegions )

# line 355 makes-time-error.cc
				(CC_tcallinit(Coordinators [MyRowRegion ][MyColRegion +1],CC_16KeepOnGoing_id),
				CC_tcall(9),
				((CC_16KeepOnGoing *)CC_mytblock)->value) ;
		}
		else {

# line 357 makes-time-error.cc
			printf ("[%d][%d]MyDone must be true, and it is %d.\n",II ,JJ ,MyDone );

# line 358 makes-time-error.cc
			(CC_tcallinit(Unfinished_Counter ,CC_8Decr_id),
			((CC_8Decr *)CC_mytblock)->arg1= 1,
			CC_tcall(10),
			((CC_8Decr *)CC_mytblock)->value) ;

# line 359 makes-time-error.cc
			CurCount =(CC_tcallinit(Unfinished_Counter ,CC_8Read_id),
			CC_tcall(11),
			((CC_8Read *)CC_mytblock)->value) ;
			if (
# line 360 makes-time-error.cc
			CurCount ==0){

# line 362 makes-time-error.cc
				All_Finish ;
				c_abort(CC_mypid);

# line 363 makes-time-error.cc

			}
			else {

				(CC_tcallinit(Coordinators [MyRowRegion ][MyColRegion ],CC_16Wait_id),
				CC_tcall(12),
				((CC_16Wait *)CC_mytblock)->value) 
# line 369 makes-time-error.cc
					;

				CurCount =(CC_tcallinit(Unfinished_Counter ,CC_8Read_id),
				CC_tcall(13),
				((CC_8Read *)CC_mytblock)->value) 
# line 374 makes-time-error.cc
					;
				if (CurCount ==0
# line 375 makes-time-error.cc
				    )
					c_abort(CC_mypid);

			}
		}

		if (
# line 383 makes-time-error.cc
		(*DidNotConverge) )
			break ;
	}

# line 390 makes-time-error.cc
	(*DidNotConverge) =1;

# line 391 makes-time-error.cc
	All_Finish ;
	CC_complete();
}

CC_main ()

# line 395 makes-time-error.cc
{
	int i ,j 
# line 396 makes-time-error.cc
	    ,did_not_converge ;
	static Real init_matrix [
# line 397 makes-time-error.cc
	    4][
# line 397 makes-time-error.cc
	    4]
# line 397 makes-time-error.cc
	    ={
		{
			1.0
# line 398 makes-time-error.cc
			    ,1.5
# line 398 makes-time-error.cc
			    ,2.0
# line 398 makes-time-error.cc
			    ,2.5
# line 398 makes-time-error.cc
		}
		,
		{
			1.5
# line 399 makes-time-error.cc
			    ,2.0
# line 399 makes-time-error.cc
			    ,2.5
# line 399 makes-time-error.cc
			    ,3.0
# line 399 makes-time-error.cc
		}
		,
		{
			2.0
# line 400 makes-time-error.cc
			    ,2.5
# line 400 makes-time-error.cc
			    ,3.0
# line 400 makes-time-error.cc
			    ,3.5
# line 400 makes-time-error.cc
		}
		,
		{
			2.5
# line 401 makes-time-error.cc
			    ,3.0
# line 401 makes-time-error.cc
			    ,3.5
# line 401 makes-time-error.cc
			    ,4.0
# line 401 makes-time-error.cc
		}
	};
	typedef Real  CC_shtypedef1;
	CC_shtypedef1 *matrix
# line 402 makes-time-error.cc
	    [4]
# line 402 makes-time-error.cc

# line 402 makes-time-error.cc
	;

	{
		int CC_j,CC_n,CC_ss;
		char *CC_s;

		CC_n=(4);
		CC_ss = (4) * sizeof(**matrix);
		CC_s = CC_shmalloc(CC_n * CC_ss);
		for (CC_j=0;CC_j<CC_n;CC_j++,CC_s+=CC_ss)
			(matrix)[CC_j] = (CC_shtypedef1 *) CC_s;
	}
	for (i =0
# line 407 makes-time-error.cc
;i <4
# line 407 makes-time-error.cc
;i ++
# line 407 makes-time-error.cc
)
		    for (j =0
# line 408 makes-time-error.cc
;j <4
# line 408 makes-time-error.cc
;j ++
# line 408 makes-time-error.cc
)
			    matrix [i ][j ]=init_matrix [i ][j ]
# line 409 makes-time-error.cc
			    ;
	Parallel_Relaxation (matrix ,0.001,1000,16,&did_not_converge ,1.0,3,2)
# line 410 makes-time-error.cc
		;
	printf ("It did ")
# line 411 makes-time-error.cc
		;
	if (did_not_converge 
# line 412 makes-time-error.cc
	    )
		printf ("not ")
# line 413 makes-time-error.cc
			;
	printf ("converge.\n")
# line 414 makes-time-error.cc
		;
	printf ("matrix is now:\n")
# line 415 makes-time-error.cc
		;
	for (i =0
# line 416 makes-time-error.cc
;i <4
# line 416 makes-time-error.cc
;i ++
# line 416 makes-time-error.cc
){
		    for (j =0
# line 417 makes-time-error.cc
;j <4
# line 417 makes-time-error.cc
;j ++
# line 417 makes-time-error.cc
)
			    printf ("%f\t",matrix [i ][j ])
# line 418 makes-time-error.cc
				;
		printf ("\n")
# line 419 makes-time-error.cc
			;
	}
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	DidNotConverge = (CC_shtypedef0 *) CC_shmalloc(sizeof(*DidNotConverge));
}

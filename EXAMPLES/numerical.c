#include <ccc/CC_kernel.h>

# line 1 numerical.cc

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

# line 69 numerical.cc

typedef float Real ;
typedef Real **RealMatrix 
# line 71 numerical.cc
;

/* PROCESS SPEC Protected_Counter */
CC_pid CC_Protected_Counter();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 80 numerical.cc
	    value;
	int arg1;
} 
CC_8Initialize;
#define CC_8Initialize_id 0

# line 80 numerical.cc

typedef int CC_headvalue2;
typedef struct {
	CC_headvalue2 
# line 81 numerical.cc
	    value;
	int arg1;
} 
CC_8Incr;
#define CC_8Incr_id 1

# line 81 numerical.cc

typedef int CC_headvalue3;
typedef struct {
	CC_headvalue3 
# line 82 numerical.cc
	    value;
	int arg1;
} 
CC_8Decr;
#define CC_8Decr_id 2

# line 82 numerical.cc

typedef int CC_headvalue4;
typedef struct {
	CC_headvalue4 
# line 83 numerical.cc
	    value;
} 
CC_8Read;
#define CC_8Read_id 3

typedef int CC_headvalue5;

# line 84 numerical.cc
typedef struct {
	CC_headvalue5 value;
} 
CC_8Quit;
#define CC_8Quit_id 4

# line 84 numerical.cc

typedef int CC_headvalue6;
typedef struct {
	CC_headvalue6 
# line 85 numerical.cc
	    value;
} 
CC_8WakeWhenAllQuit;
#define CC_8WakeWhenAllQuit_id 5

#define CC_Protected_Counter_numtrans 6

# line 86 numerical.cc

/* PROCESS SPEC Region_Task */
CC_pid CC_Region_Task();

typedef int CC_headvalue7;

# line 90 numerical.cc
typedef struct {
	CC_headvalue7 value;
	int arg1;

# line 90 numerical.cc
	int arg2;
} 
CC_15SetParameter;
#define CC_15SetParameter_id 0

# line 90 numerical.cc

#define CC_Region_Task_numtrans 1

# line 91 numerical.cc

/* PROCESS SPEC Coordinator_Task */
CC_pid CC_Coordinator_Task();

typedef int CC_headvalue8;
typedef struct {
	CC_headvalue8 
# line 95 numerical.cc
	    value;
} 
CC_17Wait;
#define CC_17Wait_id 0

typedef int CC_headvalue9;

# line 96 numerical.cc
typedef struct {
	CC_headvalue9 value;
} 
CC_17KeepOnGoing;
#define CC_17KeepOnGoing_id 1

# line 96 numerical.cc

typedef int CC_headvalue10;
typedef struct {
	CC_headvalue10 
# line 97 numerical.cc
	    value;
} 
CC_17Finish;
#define CC_17Finish_id 2

#define CC_Coordinator_Task_numtrans 3

# line 98 numerical.cc

typedef CC_pid  CC_shtypedef0;
CC_shtypedef0 
# line 101 numerical.cc
*Regions[50
# line 101 numerical.cc
]
# line 101 numerical.cc
;
typedef CC_pid  CC_shtypedef1;
CC_shtypedef1 
# line 102 numerical.cc
*Coordinators[50
# line 102 numerical.cc
]
# line 102 numerical.cc
;
CC_pid Unfinished_Counter 
# line 103 numerical.cc
;
int RowRegions ,ColRegions 
# line 104 numerical.cc
;
int NumRegions ;
int RowsPerRegion 
# line 106 numerical.cc
,ColsPerRegion ;
int cynic_max 
# line 107 numerical.cc
=1
# line 107 numerical.cc
;

RealMatrix U ;
typedef int  CC_shtypedef2;
CC_shtypedef2 
# line 110 numerical.cc
(*DidNotConverge);
Real MaxErr 
# line 111 numerical.cc
,Omega ;
int MaxIterations 
# line 112 numerical.cc
,NumberOfRegions ;
int RowHi 
# line 113 numerical.cc
,ColHi ;

Parallel_Relaxation (p_U ,p_MaxErr ,p_MaxIterations ,p_NumberOfRegions ,
p_DidNotConverge ,p_Omega ,p_RowHi ,p_ColHi )

# line 117 numerical.cc
RealMatrix p_U ;
Real p_MaxErr 
# line 118 numerical.cc
,p_Omega ;
int p_MaxIterations 
# line 119 numerical.cc
,p_NumberOfRegions ,*p_DidNotConverge 
# line 119 numerical.cc
;
int p_RowHi ,p_ColHi 
# line 120 numerical.cc
;
{

	int I ,J 
# line 141 numerical.cc
	    ;

	U =p_U 
# line 146 numerical.cc
	    ;
	MaxErr =p_MaxErr 
# line 147 numerical.cc
	    ;
	Omega =p_Omega 
# line 148 numerical.cc
	    ;
	MaxIterations =p_MaxIterations 
# line 149 numerical.cc
	    ;
	NumberOfRegions =p_NumberOfRegions 
# line 150 numerical.cc
	    ;
	RowHi =p_RowHi 
# line 151 numerical.cc
	    ;
	ColHi =p_ColHi 
# line 152 numerical.cc
	    ;

	(*DidNotConverge) =0
# line 156 numerical.cc
	    ;

	if (RowHi <2||ColHi <2
# line 161 numerical.cc
	    )
		return ;

	for (
# line 166 numerical.cc
I =1;
# line 166 numerical.cc
I <RowHi ;
# line 166 numerical.cc
I ++)
		    for (
# line 167 numerical.cc
J =1;
# line 167 numerical.cc
J <ColHi ;
# line 167 numerical.cc
J ++)

# line 168 numerical.cc
			U [I ][J ]=0.0;

# line 180 numerical.cc
	NumberOfRegions =((NumberOfRegions )<((RowHi -1)*(ColHi -1))?(NumberOfRegions ):((RowHi -1)*(ColHi -1)));

# line 181 numerical.cc
	RowRegions =((int)floor (sqrt (NumberOfRegions *(RowHi -1)/(Real)(ColHi -1))));

# line 182 numerical.cc
	ColRegions =NumberOfRegions /RowRegions ;

# line 183 numerical.cc
	NumRegions =RowRegions *ColRegions ;

# line 184 numerical.cc
	printf ("RowRegions = %d, ColRegions = %d.\n",RowRegions ,ColRegions );

# line 195 numerical.cc
	RowsPerRegion =((RowHi -1)+(RowRegions -1))/RowRegions ;

# line 196 numerical.cc
	ColsPerRegion =((ColHi -1)+(ColRegions -1))/ColRegions ;

# line 208 numerical.cc
	Unfinished_Counter =CC_Protected_Counter (0, 1 );

# line 209 numerical.cc
	(CC_tcallinit(Unfinished_Counter ,CC_8Initialize_id),
	((CC_8Initialize *)CC_mytblock)->arg1= NumRegions ,
	CC_tcall(1),
	((CC_8Initialize *)CC_mytblock)->value) ;

	for (
# line 219 numerical.cc
I =1;
# line 219 numerical.cc
I <=RowRegions ;
# line 219 numerical.cc
I ++)
		    for (
# line 220 numerical.cc
J =1;
# line 220 numerical.cc
J <=ColRegions ;
# line 220 numerical.cc
J ++){

# line 221 numerical.cc
			Regions [I ][J ]=CC_Region_Task (I ,J ,0, 2 );

# line 222 numerical.cc
			Coordinators [I ][J ]=CC_Coordinator_Task (0, 3 );
		}

	for (
# line 236 numerical.cc
I =1;
# line 236 numerical.cc
I <=RowRegions ;
# line 236 numerical.cc
I ++)
		    for (
# line 237 numerical.cc
J =1;
# line 237 numerical.cc
J <=ColRegions ;
# line 237 numerical.cc
J ++)

# line 238 numerical.cc
			(CC_tcallinit(Regions [I ][J ],CC_15SetParameter_id),
			((CC_15SetParameter *)CC_mytblock)->arg1= I ,
			((CC_15SetParameter *)CC_mytblock)->arg2= J ,
			CC_tcall(2),
			((CC_15SetParameter *)CC_mytblock)->value) ;

# line 243 numerical.cc
	(CC_tcallinit(Unfinished_Counter ,CC_8WakeWhenAllQuit_id),
	CC_tcall(3),
	((CC_8WakeWhenAllQuit *)CC_mytblock)->value) ;

# line 244 numerical.cc
	*p_DidNotConverge =(*DidNotConverge) ;
}

/* PROCESS BODY Protected_Counter */
CC_pid CC_Protected_Counter(
CC_priority, DD_createId)

# line 253 numerical.cc
int CC_priority;
unsigned DD_createId;
{
	struct {
	} 
	DD_Protected_Counter;

	int Counter 
# line 254 numerical.cc
	    ,NumberReallyAlive ;

	if (CC_fork("Protected_Counter",CC_Protected_Counter_numtrans,CC_priority,DD_createId,&DD_Protected_Counter))
		return CC_childpid;

# line 256 numerical.cc

# line 256 numerical.cc
	printf ("unixpid %d is the protected counter.\n",getpid ());
	{
		CC_tblock CC_tblock0;

# line 257 numerical.cc

# line 257 numerical.cc
		CC_accept(CC_8Initialize_id,&CC_tblock0,0,0, 1);

# line 257 numerical.cc
		{
			Counter =((CC_8Initialize *)CC_tblock0)->arg1 
# line 258 numerical.cc
			    ;
			NumberReallyAlive =((CC_8Initialize *)CC_tblock0)->arg1 
# line 259 numerical.cc
			    ;
		}
CC_treturn0: 
		CC_treturn(1);
	}

# line 260 numerical.cc

	for (;;){

		printf ("Counter is now %d.\n",Counter )
# line 264 numerical.cc
			;

		{
			CC_selectinit();
			goto CC_guard1_1; 
CC_choose1: 
			switch(CC_choose(1)) {
# line 266 numerical.cc
CC_guard1_1 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_8Incr_id,1)))
					goto CC_choose1;
				goto CC_guard1_2;
			case 1:
# line 267 numerical.cc
				{
					CC_tblock CC_tblock1;

# line 267 numerical.cc

# line 267 numerical.cc
					CC_accept(CC_8Incr_id,&CC_tblock1,0,1, 2);

# line 267 numerical.cc
					{
						Counter =Counter +((CC_8Incr *)CC_tblock1)->arg1 
# line 268 numerical.cc
						    ;
					}
CC_treturn1: 
					CC_treturn(2);
				}

# line 269 numerical.cc

				break; 
CC_guard1_2 :
# line 270 numerical.cc

				if ((CC_accept_exists=1,CC_tryaccept(CC_8Decr_id,2)))
					goto CC_choose1;
				goto CC_guard1_3;
			case 2:
# line 271 numerical.cc
				{
					CC_tblock CC_tblock2;

# line 271 numerical.cc

# line 271 numerical.cc
					CC_accept(CC_8Decr_id,&CC_tblock2,0,1, 3);
					{
						if (
# line 272 numerical.cc
						Counter !=0)

# line 273 numerical.cc
							Counter =Counter -((CC_8Decr *)CC_tblock2)->arg1 ;
						else 

# line 275 numerical.cc
						printf ("Attempt to Decr below zero!\n");
					}
CC_treturn2: 
					CC_treturn(3);
				}

# line 276 numerical.cc

				break; 
# line 277 numerical.cc
CC_guard1_3 :
				if ((CC_accept_exists=1,CC_tryaccept(CC_8Read_id,3)))
					goto CC_choose1;
				goto CC_guard1_4;
			case 3:
# line 278 numerical.cc
				{
					CC_tblock CC_tblock3;

# line 278 numerical.cc

# line 278 numerical.cc
					CC_accept(CC_8Read_id,&CC_tblock3,0,1, 4);
					{
						{
							((CC_8Read *)CC_tblock3)->value = 
# line 279 numerical.cc
							    Counter ;
							goto CC_treturn3;
						}

# line 279 numerical.cc

					}
CC_treturn3: 
					CC_treturn(4);
				}

# line 280 numerical.cc

				break; 
CC_guard1_4 :
# line 281 numerical.cc

				if ((CC_accept_exists=1,CC_tryaccept(CC_8Quit_id,4)))
					goto CC_choose1;
				goto CC_guard1_5;
			case 4:
# line 282 numerical.cc
				{
					CC_tblock CC_tblock4;

# line 282 numerical.cc
					CC_accept(CC_8Quit_id,&CC_tblock4,0,1, 5);

# line 282 numerical.cc
					{
						NumberReallyAlive --
# line 283 numerical.cc
						    ;
					}
CC_treturn4: 
					CC_treturn(5);
				}

# line 284 numerical.cc

				break; 
CC_guard1_5 :
# line 285 numerical.cc
				if ((NumberReallyAlive ==0
# line 285 numerical.cc
				    ) && 
				    (CC_accept_exists=1,CC_tryaccept(CC_8WakeWhenAllQuit_id,5)))
					goto CC_choose1;
				goto CC_guard1_6;
			case 5:
# line 286 numerical.cc
				{
					CC_tblock CC_tblock5;

# line 286 numerical.cc
					CC_accept(CC_8WakeWhenAllQuit_id,&CC_tblock5,0,1, 6);

# line 286 numerical.cc
CC_treturn5: 
					CC_treturn(6);
				}

# line 286 numerical.cc

				break; 
CC_guard1_6 :
# line 287 numerical.cc

				if (1) CC_terminate_exists=1; 
				goto CC_guard1_7;

# line 288 numerical.cc
				break; 

			}
			goto CC_endselect1;
CC_guard1_7: 
			goto CC_choose1;
CC_endselect1:;
		}

# line 289 numerical.cc

	}
	CC_complete();
}

/* PROCESS BODY Coordinator_Task */
CC_pid CC_Coordinator_Task(
CC_priority, DD_createId)

# line 294 numerical.cc
int CC_priority;
unsigned DD_createId;
{
	struct {
	} 
	DD_Coordinator_Task;

	int Had_KeepOnGoing 
# line 295 numerical.cc
	    =0
# line 295 numerical.cc
	    ;
	int Had_Finish =
# line 296 numerical.cc
	    0;

	if (CC_fork("Coordinator_Task",CC_Coordinator_Task_numtrans,CC_priority,DD_createId,&DD_Coordinator_Task))
		return CC_childpid;

# line 298 numerical.cc

# line 298 numerical.cc
	printf ("unixpid %d is a Coordinator_Task.\n",getpid ());
	for (;;)
	{
		CC_selectinit();
		goto CC_guard2_1; 
CC_choose2: 
		switch(CC_choose(2)) {
# line 300 numerical.cc
CC_guard2_1 :
# line 300 numerical.cc

			if ((CC_accept_exists=1,CC_tryaccept(CC_17KeepOnGoing_id,1)))
				goto CC_choose2;
			goto CC_guard2_2;
		case 1:
# line 301 numerical.cc
			{
				CC_tblock CC_tblock6;

# line 301 numerical.cc
				CC_accept(CC_17KeepOnGoing_id,&CC_tblock6,0,1, 7);

# line 301 numerical.cc
				{
					if ( !Had_KeepOnGoing 
# line 302 numerical.cc
					    ){
						(CC_tcallinit(Unfinished_Counter ,CC_8Incr_id),
						((CC_8Incr *)CC_mytblock)->arg1= 1,
						CC_tcall(4),
						((CC_8Incr *)CC_mytblock)->value) 
# line 303 numerical.cc
							;
						Had_KeepOnGoing =1
# line 304 numerical.cc
						    ;
					}
				}
CC_treturn6: 
				CC_treturn(7);
			}

# line 306 numerical.cc

			break; 
CC_guard2_2 :
# line 307 numerical.cc
			if ((Had_KeepOnGoing ||Had_Finish 
# line 307 numerical.cc
			    ) && 
			    (CC_accept_exists=1,CC_tryaccept(CC_17Wait_id,2)))
				goto CC_choose2;
			goto CC_guard2_3;
		case 2:
# line 308 numerical.cc
			{
				CC_tblock CC_tblock7;

# line 308 numerical.cc
				CC_accept(CC_17Wait_id,&CC_tblock7,0,1, 8);

# line 308 numerical.cc
				{
					Had_KeepOnGoing =0
# line 309 numerical.cc
					    ;
				}
CC_treturn7: 
				CC_treturn(8);
			}

# line 310 numerical.cc

			break; 
CC_guard2_3 :
# line 311 numerical.cc

			if ((CC_accept_exists=1,CC_tryaccept(CC_17Finish_id,3)))
				goto CC_choose2;
			goto CC_guard2_4;
		case 3:
# line 312 numerical.cc
			{
				CC_tblock CC_tblock8;

# line 312 numerical.cc
				CC_accept(CC_17Finish_id,&CC_tblock8,0,1, 9);

# line 312 numerical.cc
				{
					Had_Finish =1
# line 313 numerical.cc
					    ;
				}
CC_treturn8: 
				CC_treturn(9);
			}

# line 314 numerical.cc

			break; 
CC_guard2_4 :
# line 315 numerical.cc

			if (1) CC_terminate_exists=1; 
			goto CC_guard2_5;

# line 316 numerical.cc
			break; 

		}
		goto CC_endselect2;
CC_guard2_5: 
		goto CC_choose2;
CC_endselect2:;
	}

# line 317 numerical.cc

	CC_complete();
}

All_Finish ()

# line 325 numerical.cc
{
	int Rreg ,Creg 
# line 326 numerical.cc
	    ;

	for (Rreg =1
# line 328 numerical.cc
;Rreg <=RowRegions 
# line 328 numerical.cc
;Rreg ++
# line 328 numerical.cc
)
		    for (Creg =1
# line 329 numerical.cc
;Creg <=ColRegions 
# line 329 numerical.cc
;Creg ++
# line 329 numerical.cc
)
			    (CC_tcallinit(Coordinators [Rreg ][Creg ],CC_17Finish_id),
			CC_tcall(5),
			((CC_17Finish *)CC_mytblock)->value) 
# line 330 numerical.cc
				;
}

/* PROCESS BODY Region_Task */
CC_pid CC_Region_Task(II ,JJ ,
CC_priority, DD_createId)
int  II;
int  JJ;
int CC_priority;
unsigned DD_createId;
{
	struct {
		int  II;
		int  JJ;
	} 
	DD_Region_Task;

# line 334 numerical.cc

	int MyRowRegion ,MyColRegion 
# line 335 numerical.cc
	    ;
	int MyDone ,CurCount 
# line 336 numerical.cc
	    ;
	Real New_Value ,abs_error 
# line 337 numerical.cc
	    ;
	int MyRowLow ,MyRowHi 
# line 338 numerical.cc
	    ,MyColLow ,MyColHi 
# line 338 numerical.cc
	    ;
	int Iters ,I 
# line 339 numerical.cc
	    ,J ,cynic 
# line 339 numerical.cc
	    ;

	DD_Region_Task.II = II;
	DD_Region_Task.JJ = JJ;

	if (CC_fork("Region_Task",CC_Region_Task_numtrans,CC_priority,DD_createId,&DD_Region_Task))
		return CC_childpid;

# line 341 numerical.cc
	printf ("unixpid %d is a Region_Task.\n",getpid ())
# line 341 numerical.cc
		;

	{
		CC_tblock CC_tblock9;

# line 345 numerical.cc

# line 345 numerical.cc
		CC_accept(CC_15SetParameter_id,&CC_tblock9,0,0, 10);

# line 345 numerical.cc
		{
			MyRowRegion =((CC_15SetParameter *)CC_tblock9)->arg1 
# line 346 numerical.cc
			    ;
			MyColRegion =((CC_15SetParameter *)CC_tblock9)->arg2 
# line 347 numerical.cc
			    ;
		}
CC_treturn9: 
		CC_treturn(10);
	}

# line 348 numerical.cc

	MyRowLow =(MyRowRegion -1)*RowsPerRegion +1
# line 354 numerical.cc
	    ;
	MyColLow =(MyColRegion -1)*ColsPerRegion +1
# line 355 numerical.cc
	    ;

	MyRowHi =((RowHi -1)<(MyRowLow +RowsPerRegion -1)?(RowHi -1):(MyRowLow +RowsPerRegion -1))
# line 360 numerical.cc
		;
	MyColHi =((ColHi -1)<(MyColLow +ColsPerRegion -1)?(ColHi -1):(MyColLow +ColsPerRegion -1))
# line 361 numerical.cc
		;
	for (Iters =1
# line 362 numerical.cc
;Iters <=MaxIterations 
# line 362 numerical.cc
;Iters ++
# line 362 numerical.cc
){
		    MyDone =1
# line 363 numerical.cc
		    ;

		for (I =MyRowLow 
# line 367 numerical.cc
;I <=MyRowHi 
# line 367 numerical.cc
;I ++
# line 367 numerical.cc
){
			    for (J =MyColLow 
# line 368 numerical.cc
;J <=MyColHi 
# line 368 numerical.cc
;J ++
# line 368 numerical.cc
){
				    for (cynic =0
# line 369 numerical.cc
;cynic <cynic_max 
# line 369 numerical.cc
;cynic ++
# line 369 numerical.cc
){

					New_Value =U [I ][J ]+Omega *0.25*(U [I -1][J ]+U [I ][J -1]+U [I +1][J ]+U [I ][J +1]-4.0*U [I ][J ])
# line 371 numerical.cc
						;
					abs_error =((New_Value -U [I ][J ])<0.0? -(New_Value -U [I ][J ]):(New_Value -U [I ][J ]))
# line 372 numerical.cc
						;
					if (abs_error >0.0
# line 373 numerical.cc
					    )
						break ;
					if (cynic +1<cynic_max 
# line 375 numerical.cc
					    )
						printf ("[%d,%d] I don't believe it.\n",II ,JJ )
# line 376 numerical.cc
							;
				}
				if (abs_error >=MaxErr 
# line 378 numerical.cc
				    )
					MyDone =0
# line 379 numerical.cc
					    ;
				U [I ][J ]=New_Value 
# line 380 numerical.cc
				    ;
			}
		}
		if ( !MyDone 
# line 383 numerical.cc
		    ){
			if (MyRowRegion !=1
# line 384 numerical.cc
			    )
				(CC_tcallinit(Coordinators [MyRowRegion -1][MyColRegion ],CC_17KeepOnGoing_id),
				CC_tcall(6),
				((CC_17KeepOnGoing *)CC_mytblock)->value) 
# line 385 numerical.cc
					;

			if (MyRowRegion !=RowRegions 
# line 387 numerical.cc
			    )
				(CC_tcallinit(Coordinators [MyRowRegion +1][MyColRegion ],CC_17KeepOnGoing_id),
				CC_tcall(7),
				((CC_17KeepOnGoing *)CC_mytblock)->value) 
# line 388 numerical.cc
					;

			if (MyColRegion !=1
# line 390 numerical.cc
			    )
				(CC_tcallinit(Coordinators [MyRowRegion ][MyColRegion -1],CC_17KeepOnGoing_id),
				CC_tcall(8),
				((CC_17KeepOnGoing *)CC_mytblock)->value) 
# line 391 numerical.cc
					;

			if (MyColRegion !=ColRegions 
# line 393 numerical.cc
			    )
				(CC_tcallinit(Coordinators [MyRowRegion ][MyColRegion +1],CC_17KeepOnGoing_id),
				CC_tcall(9),
				((CC_17KeepOnGoing *)CC_mytblock)->value) 
# line 394 numerical.cc
					;
		}
		else {
			(CC_tcallinit(Unfinished_Counter ,CC_8Decr_id),
			((CC_8Decr *)CC_mytblock)->arg1= 1,
			CC_tcall(10),
			((CC_8Decr *)CC_mytblock)->value) 
# line 396 numerical.cc
				;
			CurCount =(CC_tcallinit(Unfinished_Counter ,CC_8Read_id),
			CC_tcall(11),
			((CC_8Read *)CC_mytblock)->value) 
# line 397 numerical.cc
				;
			if (CurCount ==0
# line 398 numerical.cc
			    ){

				All_Finish 
# line 400 numerical.cc
				    ;
				(CC_tcallinit(Unfinished_Counter ,CC_8Quit_id),
				CC_tcall(12),
				((CC_8Quit *)CC_mytblock)->value) 
# line 401 numerical.cc
					;
				c_abort(CC_mypid);

			}
			else {

# line 408 numerical.cc
				(CC_tcallinit(Coordinators [MyRowRegion ][MyColRegion ],CC_17Wait_id),
				CC_tcall(13),
				((CC_17Wait *)CC_mytblock)->value) ;

# line 413 numerical.cc
				CurCount =(CC_tcallinit(Unfinished_Counter ,CC_8Read_id),
				CC_tcall(14),
				((CC_8Read *)CC_mytblock)->value) ;
				if (
# line 414 numerical.cc
				CurCount ==0){

# line 415 numerical.cc
					(CC_tcallinit(Unfinished_Counter ,CC_8Quit_id),
					CC_tcall(15),
					((CC_8Quit *)CC_mytblock)->value) ;
					c_abort(CC_mypid);

# line 416 numerical.cc

				}
			}
		}

		if ((*DidNotConverge) 
# line 424 numerical.cc
		    )
			break ;
	}

	(*DidNotConverge) =1
# line 431 numerical.cc
	    ;
	All_Finish 
# line 432 numerical.cc
	    ;
	(CC_tcallinit(Unfinished_Counter ,CC_8Quit_id),
	CC_tcall(16),
	((CC_8Quit *)CC_mytblock)->value) 
# line 433 numerical.cc
		;
	CC_complete();
}

CC_main (argc ,argv )
int argc 
# line 437 numerical.cc
;
char **argv ;
{
	int i 
# line 440 numerical.cc
	    ,j ,did_not_converge 
# line 440 numerical.cc
	    ;
	static Real init_matrix [4
# line 441 numerical.cc
	    ][4
# line 441 numerical.cc
	    ]={
		{
# line 442 numerical.cc
			1.0,
# line 442 numerical.cc
			1.5,
# line 442 numerical.cc
			2.0,
# line 442 numerical.cc
			2.5		}
		,
		{
# line 443 numerical.cc
			1.5,
# line 443 numerical.cc
			2.0,
# line 443 numerical.cc
			2.5,
# line 443 numerical.cc
			3.0		}
		,
		{
# line 444 numerical.cc
			2.0,
# line 444 numerical.cc
			2.5,
# line 444 numerical.cc
			3.0,
# line 444 numerical.cc
			3.5		}
		,
		{
# line 445 numerical.cc
			2.5,
# line 445 numerical.cc
			3.0,
# line 445 numerical.cc
			3.5,
# line 445 numerical.cc
			4.0		}
	};
	typedef Real  CC_shtypedef3;
	CC_shtypedef3 
# line 446 numerical.cc
	    *matrix[4
# line 446 numerical.cc
	    ]
# line 446 numerical.cc
	    ;

	{
		int CC_j,CC_n,CC_ss;
		char *CC_s;

		CC_n=(4);
		CC_ss = (4) * sizeof(**matrix);
		CC_s = CC_shmalloc(CC_n * CC_ss);
		for (CC_j=0;CC_j<CC_n;CC_j++,CC_s+=CC_ss)
			(matrix)[CC_j] = (CC_shtypedef3 *) CC_s;
	}

# line 448 numerical.cc
	c_core_dump_on_error (1);

	if (
# line 453 numerical.cc
	argc >1)

# line 454 numerical.cc
		sscanf (argv [1],"%d",&cynic_max );

	for (
# line 458 numerical.cc
i =0;
# line 458 numerical.cc
i <4;
# line 458 numerical.cc
i ++)
		    for (
# line 459 numerical.cc
j =0;
# line 459 numerical.cc
j <4;
# line 459 numerical.cc
j ++)

# line 460 numerical.cc
			matrix [i ][j ]=init_matrix [i ][j ];

# line 461 numerical.cc
	Parallel_Relaxation (matrix ,0.001,1000,16,&did_not_converge ,1.0,3,3);

# line 462 numerical.cc
	printf ("It did ");
	if (
# line 463 numerical.cc
	did_not_converge )

# line 464 numerical.cc
		printf ("not ");

# line 465 numerical.cc
	printf ("converge.\n");

# line 466 numerical.cc
	printf ("matrix is now:\n");
	for (
# line 467 numerical.cc
i =0;
# line 467 numerical.cc
i <4;
# line 467 numerical.cc
i ++){
		    for (
# line 468 numerical.cc
j =0;
# line 468 numerical.cc
j <4;
# line 468 numerical.cc
j ++)

# line 469 numerical.cc
			printf ("%f\t",matrix [i ][j ]);

# line 470 numerical.cc
		printf ("\n");
	}
}
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

	CC_n=(50);
	CC_ss = (50) * sizeof(**Regions);
	CC_s = CC_shmalloc(CC_n * CC_ss);
	for (CC_j=0;CC_j<CC_n;CC_j++,CC_s+=CC_ss)
		(Regions)[CC_j] = (CC_shtypedef0 *) CC_s;
	CC_n=(50);
	CC_ss = (50) * sizeof(**Coordinators);
	CC_s = CC_shmalloc(CC_n * CC_ss);
	for (CC_j=0;CC_j<CC_n;CC_j++,CC_s+=CC_ss)
		(Coordinators)[CC_j] = (CC_shtypedef1 *) CC_s;
	DidNotConverge = (CC_shtypedef2 *) CC_shmalloc(sizeof(*DidNotConverge));
}

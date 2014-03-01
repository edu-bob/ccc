#include <ccc/CC_kernel.h>

# line 1 pragma.cc

/* PROCESS SPEC doof */
CC_pid CC_doof();

typedef int CC_headvalue1;
typedef struct {
	CC_headvalue1 
# line 4 pragma.cc
	    value;
} 
CC_8me;
#define CC_8me_id 0

#define CC_doof_numtrans 1

# line 5 pragma.cc

/* PROCESS BODY doof */
CC_pid CC_doof(
CC_priority, DD_createId)

# line 8 pragma.cc
int CC_priority;
unsigned DD_createId;
{

	if (CC_fork("doof",CC_doof_numtrans,CC_priority,DD_createId))
		return CC_childpid;

# line 9 pragma.cc
	{
		CC_selectinit();
		goto CC_guard1_1; 
CC_choose1: 
		switch(CC_choose(1)) {
# line 9 pragma.cc
CC_guard1_1 :
# line 9 pragma.cc

			if ((CC_accept_exists=1,CC_tryaccept(CC_8me_id,1)))
				goto CC_choose1;
			goto CC_guard1_2;
		case 1:
# line 10 pragma.cc
			{
				CC_tblock CC_tblock0;

# line 10 pragma.cc
				CC_accept(CC_8me_id,&CC_tblock0,0,1, 1);

# line 10 pragma.cc
CC_treturn0: 
				CC_treturn(1);
			}

# line 10 pragma.cc

			break; 
		}
		goto CC_endselect1;
CC_guard1_2: 
		goto CC_choose1;
CC_endselect1:;
	}

# line 11 pragma.cc

	{
		CC_selectinit();
		goto CC_guard2_1; 
CC_choose2: 
		switch(CC_choose(2)) {
# line 13 pragma.cc
CC_guard2_1 :
# line 13 pragma.cc

			if ((CC_accept_exists=1,CC_tryaccept(CC_8me_id,1)))
				goto CC_choose2;
			goto CC_guard2_2;
		case 1:
# line 14 pragma.cc
			{
				CC_tblock CC_tblock1;

# line 14 pragma.cc
				CC_accept(CC_8me_id,&CC_tblock1,0,1, 2);

# line 14 pragma.cc
CC_treturn1: 
				CC_treturn(2);
			}

# line 14 pragma.cc

			break; 
		}
		goto CC_endselect2;
CC_guard2_2: 
		goto CC_choose2;
CC_endselect2:;
	}

# line 15 pragma.cc

	{
		CC_selectinit();
		goto CC_guard3_1; 
CC_choose3: 
		switch(CC_choose(3)) {
# line 16 pragma.cc
CC_guard3_1 :
# line 16 pragma.cc

			if ((CC_accept_exists=1,CC_tryaccept(CC_8me_id,1)))
				goto CC_choose3;
			goto CC_guard3_2;
		case 1:
# line 17 pragma.cc
			{
				CC_tblock CC_tblock2;

# line 17 pragma.cc
				CC_accept(CC_8me_id,&CC_tblock2,0,1, 3);

# line 17 pragma.cc
CC_treturn2: 
				CC_treturn(3);
			}

# line 17 pragma.cc

			break; 
		}
		goto CC_endselect3;
CC_guard3_2: 
		goto CC_choose3;
CC_endselect3:;
	}

# line 18 pragma.cc

	{
		int h 
# line 21 pragma.cc
		    ,i ,j 
# line 21 pragma.cc
		    ,k ;
		{
			float j 
# line 23 pragma.cc
			    ,k ,l 
# line 23 pragma.cc
			    ;

			/*
			 *  exiting block in which the following were declared:
			 *  |name                context chain   num     obj     alias               |
			 *  |j                   0       0       6       13                          |
			 *  |k                   0       0       6       13                          |
			 *  |l                   0       0       6       13                          |
			 */
		}

		/*
		 *  exiting block in which the following were declared:
		 *  |name                context chain   num     obj     alias               |
		 *  |h                   0       0       6       14                          |
		 *  |i                   0       0       6       14                          |
		 *  |j                   0       0       6       14                          |
		 *  |k                   0       0       6       14                          |
		 */
	}

	state 274, char 037777777777
	    reduce 508
	    state 268, char 037777777777
	    state 102, char 037777777777
	    reduce 544
	    state 375, char 037777777777
	    reduce 224
	    {
		CC_selectinit();
		goto CC_guard4_1; 
CC_choose4: 
		switch(CC_choose(4)) {
# line 27 pragma.cc
			state 481, char 037777777777
			    reduce 226
CC_guard4_1 :
			    state 565, char 037777777777

			reduce 230
			    if (state 617, char 0435
			    state 277, char 037777777777
			    reduce 465
			    state 276, char 037777777777
			    state 61, char 037777777777
			    reduce 495
			    state 381, char 037777777777
			    reduce 246
			    state 656, char 037777777777
			    reduce 237
			    (CC_accept_exists=1,CC_tryaccept(CC_8me_id,1)))
				goto CC_choose4;
			goto CC_guard4_2;
		case 1:
# line 28 pragma.cc
			state 690, char 037777777777
			    reduce 247
			    {
				CC_tblock CC_tblock3;

# line 28 pragma.cc
				state 378, char 037777777777
				    state 65, char 037777777777
				    reduce 540
				    state 482, char 037777777777
				    reduce 250
				    state 566, char 051
				    state 132, char 037777777777
				    reduce 541
				    state 619, char 037777777777
				    reduce 248

# line 28 pragma.cc
				state 659, char 037777777777
				    reduce 255
				    CC_accept(CC_8me_id,&CC_tblock3,0,1, 4);
				state 692, char 073
				    state 83, char 037777777777
				    reduce 548
				    state 712, char 037777777777
				    reduce 258
				    state 711, char 037777777777
				    reduce 249

				/*
				 *  exiting block in which the following were declared:
				 *  |name                context chain   num     obj     alias               |
				 */
CC_treturn3: 
				CC_treturn(4);
			}

# line 28 pragma.cc
			state 709, char 037777777777
			    reduce 210
			    state 727, char 037777777777

			reduce 238
			    state 652, char 0175
			    reduce 227
			    break; 
# line 29 pragma.cc
			state 564, char 0175
			    reduce 239
			    state 615, char 0175
			    reduce 243
			    state 648, char 0175
			    state 223, char 037777777777
			    reduce 545
			    state 684, char 037777777777
			    reduce 225

		}
		goto CC_endselect4;
CC_guard4_2: 
		goto CC_choose4;
CC_endselect4:;
	}

# line 29 pragma.cc
	state 250, char 037777777777
	    reduce 203
	    state 234, char 037777777777
	    reduce 201
	    state 224, char 037777777777
	    reduce 211
	    state 715, char 037777777777

	{
		CC_selectinit();
		goto CC_guard5_1; 
CC_choose5: 
		switch(CC_choose(5)) {
# line 31 pragma.cc
CC_guard5_1 :
			if ((CC_accept_exists=1,CC_tryaccept(CC_8me_id,1)))
				goto CC_choose5;
			goto CC_guard5_2;
		case 1:
# line 32 pragma.cc
			{
				CC_tblock CC_tblock4;

# line 32 pragma.cc

# line 32 pragma.cc
				CC_accept(CC_8me_id,&CC_tblock4,0,1, 5);

				/*
				 *  exiting block in which the following were declared:
				 *  |name                context chain   num     obj     alias               |
				 */
CC_treturn4: 
				CC_treturn(5);
			}

# line 32 pragma.cc

			break; 
# line 33 pragma.cc

		}
		goto CC_endselect5;
CC_guard5_2: 
		goto CC_choose5;
CC_endselect5:;
	}

# line 33 pragma.cc

	{
		CC_selectinit();
		goto CC_guard6_1; 
CC_choose6: 
		switch(CC_choose(6)) {
# line 34 pragma.cc
CC_guard6_1 :
			if ((CC_accept_exists=1,CC_tryaccept(CC_8me_id,1)))
				goto CC_choose6;
			goto CC_guard6_2;
		case 1:
# line 35 pragma.cc
			{
				CC_tblock CC_tblock5;

# line 35 pragma.cc

# line 35 pragma.cc
				CC_accept(CC_8me_id,&CC_tblock5,0,1, 6);

				/*
				 *  exiting block in which the following were declared:
				 *  |name                context chain   num     obj     alias               |
				 */
CC_treturn5: 
				CC_treturn(6);
			}

# line 35 pragma.cc

			break; 
# line 36 pragma.cc

		}
		goto CC_endselect6;
CC_guard6_2: 
		goto CC_choose6;
CC_endselect6:;
	}

# line 36 pragma.cc

	/*
	 *  exiting block in which the following were declared:
	 *  |name                context chain   num     obj     alias               |
	 *  |c_mypid             0       25      8       3                           |
	 */
	CC_complete();
}

CC_main ()
{

# line 41 pragma.cc
	printf ("Don't look at the executable; look at the compilation\n");

# line 42 pragma.cc
	printf ("and at pragma.c.\n");
}

/*
 *  exiting block in which the following were declared:
 *  |name                context chain   num     obj     alias               |
 *  |CC_main             0       25      6       0                           |
 *  |c_abort             0       25      6       14                          |
 *  |c_active            0       25      6       14                          |
 *  |c_associate         0       25      6       14                          |
 *  |c_changepriority    0       25      6       14                          |
 *  |c_completed         0       25      6       14                          |
 *  |c_getpriority       0       25      6       14                          |
 *  |c_invalid           0       25      6       14                          |
 *  |c_nullpid           0       0       5       3                           |
 *  |printf              0       25      6       0                           |
 *  |anytype             1       0       4       3                           |
 *  |doof                1       0       8       3                           |
 *  |me                  8       0       9       14                          |
 *  |0                   9       0       0       0                           |
 *  |=                   9       0       6       14                          |
 */
CC_shinit()
{
	int CC_j,CC_n,CC_ss;
	char *CC_s;

}

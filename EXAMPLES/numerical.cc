/* numerical.cc */

/*
 *  This is a program adapted from a very flawed Ada routine which performed
 *  relaxation over a grid. Two bugs have been brutally hacked away:
 *  The first concerned the fact that in part of the code, the
 *  protected counter would be decremented, and THEN its value would be
 *  checked. The problem was that the counter can go below zero,
 *  and thus you would have:
 *
 *	(counter = 1)
 *			A: decrement
 *	(counter = 0)
 *						B: decrement
 *	(counter = -1)
 *			A: counter = 0? no...
 *						B: counter = 0? no...
 *
 *  Thus the program would not complete. The hack for this was to
 *  ignore decrements when counter=0. The second bug is the one discussed
 *  in makes-time-error.cc. The hack for this is a "cynic" factor
 *  which forces reevaluation when an error of exactly 0.0 is found.
 *  The need for this factor (as given via the command line) is
 *  seen in the output below. The first time, no reevaluation is done;
 *  the second time, errors of zero cause the calculation to be reevaluated
 *  up to three times.
 *

% numerical
RowRegions = 2, ColRegions = 2.
Attempt to Decr below zero!
Attempt to Decr below zero!
It did converge.
matrix is now:
1.000000	1.500000	2.000000	2.500000	
1.500000	1.985275	2.495485	3.000000	
2.000000	2.492638	2.997031	3.500000	
2.500000	3.000000	3.500000	4.000000	

% numerical 3
RowRegions = 2, ColRegions = 2.
[1,1] I don't believe it.
[1,1] I don't believe it.
[1,1] I don't believe it.
[1,1] I don't believe it.
[1,1] I don't believe it.
[1,1] I don't believe it.
[1,1] I don't believe it.
[1,1] I don't believe it.
[1,1] I don't believe it.
[1,2] I don't believe it.
[1,1] I don't believe it.
[1,1] I don't believe it.
[1,1] I don't believe it.
[1,2] I don't believe it.
[1,2] I don't believe it.
[2,1] I don't believe it.
It did converge.
matrix is now:
1.000000	1.500000	2.000000	2.500000	
1.500000	1.999798	2.499407	3.000000	
2.000000	2.499785	2.999798	3.500000	
2.500000	3.000000	3.500000	4.000000	

 *
 */

#include <math.h>
typedef float Real;
typedef Real **RealMatrix;

#define Abs(i)		((i)<0.0 ? -(i) : (i))
#define Min(a,b)	((a)<(b) ? (a) : (b))
#define Floor(x)	((int)floor(x))
#define Sqrt(x)		sqrt(x)

process spec Protected_Counter()
{
    trans Initialize(int);
    trans Incr(int);		/* no default value */
    trans Decr(int);		/* no default value */
    trans int Read();		/* value returned as value, not out param */
    trans Quit();
    trans WakeWhenAllQuit();
};

process spec Region_Task(int,int)
{
    trans SetParameter(int,int);
};

process spec Coordinator_Task()
{
    trans Wait();
    trans KeepOnGoing();
    trans Finish();
};

#define MaxDim 50
shared process Region_Task Regions[MaxDim][MaxDim];
shared process Coordinator_Task Coordinators[MaxDim][MaxDim];
process Protected_Counter Unfinished_Counter;
int RowRegions, ColRegions;
int NumRegions;
int RowsPerRegion, ColsPerRegion;
int cynic_max=1;

RealMatrix U;
shared int DidNotConverge;
Real MaxErr,Omega;
int MaxIterations,NumberOfRegions;
int RowHi,ColHi;

Parallel_Relaxation(p_U, p_MaxErr, p_MaxIterations, p_NumberOfRegions,
		    p_DidNotConverge, p_Omega, p_RowHi, p_ColHi)
RealMatrix p_U;
Real p_MaxErr,p_Omega;
int p_MaxIterations,p_NumberOfRegions,*p_DidNotConverge;
int p_RowHi,p_ColHi; /* highest legal index in p_U */
{
/*
 *  MaxErr determines when we have converged (i.e., we have converged
 *	when the change in the value of all point <= MaxErr)
 *  MaxIterations is a limit on how many iterations to perform.  If
 *	we perform this many iterations without converging, then we set
 *	DidNotConverge to True and return.
 *  NumberOfRegions is the maximum number of Ada tasks in use.
 *  Omega is the acceleration parameter to the successive overrelaxation
 *	formula.
 */

#define RowLo	0
#define ColLo	0
#define LenInteriorRows (RowHi-1)
#define LenInteriorCols (ColHi-1)
/*
 *  subtype InteriorRows is Integer range RowLo+1..RowHi-1;
 *  subtype InteriorCols is Integer range ColLo+1..ColHi-1;
 */
    int I,J;

/*
 *  Copy local parameters into global variables:
 */
    U = p_U;
    MaxErr = p_MaxErr;
    Omega = p_Omega;
    MaxIterations = p_MaxIterations;
    NumberOfRegions = p_NumberOfRegions;
    RowHi = p_RowHi;
    ColHi = p_ColHi;
/*
 *  Initially, assume that the system will converge:
 */
    DidNotConverge=0;

/*
 *  See if the matrix U has any interior points.  If not, return:
 */
    if (RowHi < 2 || ColHi < 2)
	return;
/*
 *  We zero the interior points initially:
 */
    for (I=1; I<RowHi; I++)
	for (J=1; J<ColHi; J++)
	    U[I][J] = 0.0;
/*
 *  Determine the layout of the regions on the matrix.
 *  Each region is itself a rectangular sub-matrix.  We lay down a
 *  rectangular array of regions on the top of the matrix.  The
 *  array is RowRegions by ColRegions:
 *
 *  But first... make sure NumberOfRegions isn't too LARGE!
 */

/*    RowRegions = Floor(Sqrt((Real)NumberOfRegions));*/

    NumberOfRegions = Min(NumberOfRegions, LenInteriorRows * LenInteriorCols);
    RowRegions = Floor(Sqrt(NumberOfRegions*LenInteriorRows/(Real)LenInteriorCols));
    ColRegions = NumberOfRegions / RowRegions;
    NumRegions = RowRegions * ColRegions;
    printf("RowRegions = %d, ColRegions = %d.\n",RowRegions,ColRegions);
/*
 *  E.g. for NumberOfRegions = 33, we get 5, 6, and 30.
 *  Only the 30 regions are actually used.
 */

/*
 *  Each region is a rectangle of RowsPerRegions rows by
 *  ColsPerRegion columns.  The regions at the right edge
 *  and the bottom may be smaller however:
 */
    RowsPerRegion = (LenInteriorRows + (RowRegions - 1)) / RowRegions;
    ColsPerRegion = (LenInteriorCols + (ColRegions - 1)) / ColRegions;
/*
 *  Now that we know how many tasks we actually want, call
 *  a procedure which declares them & does the relaxation:
 *
 *  (Actually, the body of ParRelax_Inner_Proc is inserted here.)
 */

/*
 *  Create the unfinished task counter. The count of unfinished
 *  tasks is initially all of the region tasks:
 */
    Unfinished_Counter = create Protected_Counter();
    Unfinished_Counter.Initialize(NumRegions);
/*
 *  Initialize arrays with processes:
 *  (There is an implicit bound not enforced here, set by MaxDim!)
 */

/*
 *  A subtle bug... why does not having Regions and Coordinators as
 *  shared arrays make the following loop die?
 */
    for (I=1; I<=RowRegions; I++)
	for (J=1; J<=ColRegions; J++) {
	    Regions[I][J] = create Region_Task(I,J);
	    Coordinators[I][J] = create Coordinator_Task();
	}
/*
 *  Answer:
 *  Many times when we are dealing with read-only data, such as
 *  process ids, we can use a non-shared array because we don't care
 *  that updates are private. HOWEVER, in this loop,
 *  the earlier created processes don't get the later processes
 *  in their tables!!!!
 */

/*
 *  Set the parameters of the region tasks:
 */
    for (I=1; I<=RowRegions; I++)
	for (J=1; J<=ColRegions; J++)
	    Regions[I][J].SetParameter(I,J);
/*
 *  Wait until there are no more unfinished tasks,
 *  set the convergence flag, and return.
 */
    Unfinished_Counter.WakeWhenAllQuit();
    *p_DidNotConverge = DidNotConverge;
}


/*
 *  And now... the process bodies.
 */

process body Protected_Counter()
{
    int Counter,NumberReallyAlive;

printf("unixpid %d is the protected counter.\n",getpid());
    accept Initialize(Z) {
	Counter = Z;
	NumberReallyAlive = Z;
    }
    for (;;) {

/* this next line needs to be here to make things work!!!??? */
	printf("Counter is now %d.\n",Counter);

	select {
	    accept Incr(Z) {
		Counter = Counter + Z;
	    }
	or
	    accept Decr(Z) {
	        if (Counter != 0) /* this needs to be here to avoid time error */
		    Counter = Counter - Z;
		else
		    printf("Attempt to Decr below zero!\n");
	    }
	or
	    accept Read() {
		treturn Counter;
	    }
	or
	    accept Quit() {
		NumberReallyAlive--;
	    }
	or (NumberReallyAlive == 0):
	    accept WakeWhenAllQuit();
	or
	    terminate;
	}
    }
}

process body Coordinator_Task()
{
    int Had_KeepOnGoing=0;
    int Had_Finish=0;

printf("unixpid %d is a Coordinator_Task.\n",getpid());
    for (;;)
	select {
	    accept KeepOnGoing() {
		if (!Had_KeepOnGoing) {
		    Unfinished_Counter.Incr(1);
		    Had_KeepOnGoing=1;
		}
	    }
	or (Had_KeepOnGoing || Had_Finish):
	    accept Wait() {
		Had_KeepOnGoing=0;
	    }
	or
	    accept Finish() {
		Had_Finish=1;
	    }
	or
	    terminate;
	}
}

/*
 *  Definition of All_Finish, used by the Region_Tasks.
 *  All_Finish calls the Finish entries of all coordinators.
 */
All_Finish()
{
    int Rreg, Creg;

    for (Rreg=1; Rreg<=RowRegions; Rreg++)
	for (Creg=1; Creg<=ColRegions; Creg++)
	    Coordinators[Rreg][Creg].Finish();
}

process body Region_Task(II,JJ)
{
    int MyRowRegion, MyColRegion;
    int MyDone, CurCount;
    Real New_Value, abs_error;
    int MyRowLow, MyRowHi, MyColLow, MyColHi;
    int Iters,I,J,cynic;

printf("unixpid %d is a Region_Task.\n",getpid());
/*
 *  Task starts by finding out what region it has been assigned:
 */
    accept SetParameter(SetMyRowRegion, SetMyColRegion) {
	MyRowRegion = SetMyRowRegion;
	MyColRegion = SetMyColRegion;
    }
    
/*
 *  Compute the boundaries of my region.  These are
 *  the points that will be computed by me:
 */
    MyRowLow = (MyRowRegion - 1) * RowsPerRegion + 1;
    MyColLow = (MyColRegion - 1) * ColsPerRegion + 1;
/*
 *  If we're at the bottom edge, then MyRowHi should not exceed
 *  InteriorRows'LAST; likewise, if we're at the right edge.
 */
    MyRowHi = Min(RowHi - 1, MyRowLow + RowsPerRegion - 1);
    MyColHi = Min(ColHi - 1, MyColLow + ColsPerRegion - 1);
    for (Iters=1; Iters<=MaxIterations; Iters++) {
	MyDone=1;
/*
 *	Compute a new value for each point in my region:
 */
	for (I=MyRowLow; I<=MyRowHi; I++) {
	    for (J=MyColLow; J<=MyColHi; J++) {
		for (cynic=0;cynic<cynic_max;cynic++) {
		    New_Value = U[I][J] + Omega * 0.25 *
		      (U[I-1][J] + U[I][J-1] + U[I+1][J] + U[I][J+1] - 4.0*U[I][J]);
		    abs_error = Abs(New_Value - U[I][J]);
		    if (abs_error > 0.0)
			break;
		    if (cynic+1 < cynic_max)
			printf("[%d,%d] I don't believe it.\n",II,JJ);
		}
		if (abs_error >= MaxErr)
		    MyDone = 0;
		U[I][J] = New_Value;
	    }
	}
        if (!MyDone) {  /*Tell my neighbors to keep on going */
	    if (MyRowRegion != 1)
		Coordinators[MyRowRegion-1][MyColRegion].KeepOnGoing();

	    if (MyRowRegion != RowRegions)
		Coordinators[MyRowRegion+1][MyColRegion].KeepOnGoing();

	    if (MyColRegion != 1)
		Coordinators[MyRowRegion][MyColRegion-1].KeepOnGoing();

	    if (MyColRegion != ColRegions)
		Coordinators[MyRowRegion][MyColRegion+1].KeepOnGoing();
	} else {
	    Unfinished_Counter.Decr(1);
	    CurCount = Unfinished_Counter.Read();
	    if (CurCount == 0) {
		/* We're all done.  Wake up everybody who's sleeping: */
		All_Finish;
		Unfinished_Counter.Quit();
		terminate;
	    } else {
/*
 *		Wait to hear of some change from my neighbors, or
 *		for all tasks to finish.
 */
		Coordinators[MyRowRegion][MyColRegion].Wait();
/*
 *		We are all awakened.  See whether this is
 *		because of a KeepOnGoing message:
 */
		CurCount = Unfinished_Counter.Read();
		if (CurCount == 0) {
		    Unfinished_Counter.Quit();
		    terminate;
		}
	    }
	}
/*
 *	See if some other task has taken too many
 *	iterations already and if so stop iterating.
 */
	if (DidNotConverge)
	    break;
    }
/*
 *  Here iff some task (either my task or some
 *  other task) has taken too many iterations.
 */
    DidNotConverge = 1;
    All_Finish;
    Unfinished_Counter.Quit();
}

main(argc, argv)
int argc;
char **argv;
{
    int i,j,did_not_converge;
    static Real init_matrix[4][4] = {
	{1.0,	1.5,	2.0,	2.5},
	{1.5,	2.0,	2.5,	3.0},
	{2.0,	2.5,	3.0,	3.5},
	{2.5,	3.0,	3.5,	4.0} };
    shared Real matrix[4][4];

    c_core_dump_on_error(1);

/*
 *  Set level of cynicism, if given on command line.
 */
    if (argc>1)
	sscanf(argv[1],"%d",&cynic_max);
/*
 *  Shared arrays cannot be initialized, so we have to do it ourselves.
 */
    for (i=0; i<4; i++)
	for (j=0; j<4; j++)
	    matrix[i][j] = init_matrix[i][j];
    Parallel_Relaxation(matrix, 0.001, 1000, 16, &did_not_converge, 1.0, 3, 3);
    printf("It did ");
    if (did_not_converge)
	printf("not ");
    printf("converge.\n");
    printf("matrix is now:\n");
    for (i=0; i<4; i++) {
	for (j=0; j<4; j++)
	    printf("%f\t",matrix[i][j]);
	printf("\n");
    }
}

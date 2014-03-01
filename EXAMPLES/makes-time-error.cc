/* makes-time-error.cc */

/*
 *  This is a version of numerical.cc before being hacked into working.
 *  This program will (sometimes, give it a few runs) produce the
 *  very subtle false convergence bug, where process A produces the
 *  same values in a row (as the data it uses have not been updated in
 *  between the two calculations; meanwhile process B calculates one value
 *  from the first of A's new values, and one from the second.
 *  Thus both A and B (the only processes) have produced two prefectly
 *  consistant results, and thus they both let eachother quit,
 *  BEFORE any real convergence has taken place!!!
 *
 *  Output when the bug manifests itself:
 *

% makes-time-error 
R,C,NumRegions = 2, 1, 2.
[1][1] MyRowLow=1 MyRowHi=1 MyColLow=1 MyColHi=1.
[2][1] MyRowLow=2 MyRowHi=2 MyColLow=1 MyColHi=1.
Abs(New_Value - U[1][1])=1.375000, New_Value=1.375000, U[1][1]=0.000000.
Abs(New_Value - U[2][1])=2.000000, New_Value=2.000000, U[2][1]=0.000000.
[1][1]MyDone must be false, and it is 0.
[2][1]MyDone must be false, and it is 0.
Abs(New_Value - U[1][1])=0.500000, New_Value=1.875000, U[1][1]=1.375000.
[1][1]MyDone must be false, and it is 0.
after Incr, Counter is now 3.
Abs(New_Value - U[1][1])=0.000000, New_Value=1.875000, U[1][1]=1.875000.
[1][1]MyDone must be true, and it is 1.
Abs(New_Value - U[2][1])=0.468750, New_Value=2.468750, U[2][1]=2.000000.
after Decr, Counter is now 2.
[2][1]MyDone must be false, and it is 0.
Abs(New_Value - U[2][1])=0.000000, New_Value=2.468750, U[2][1]=2.468750.
Abs(New_Value - U[1][1])=0.117188, New_Value=1.992188, U[1][1]=1.875000.
[2][1]MyDone must be true, and it is 1.
[1][1]MyDone must be false, and it is 0.
after Decr, Counter is now 1.
Abs(New_Value - U[1][1])=0.000000, New_Value=1.992188, U[1][1]=1.992188.
[1][1]MyDone must be true, and it is 1.
after Decr, Counter is now 0.
It did converge.
matrix is now:
1.000000	1.500000	2.000000	2.500000	
1.500000	1.992188	2.500000	3.000000	
2.000000	2.468750	3.000000	3.500000	
2.500000	3.000000	3.500000	4.000000	
%

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
    trans WakeWhenZero();
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
process Region_Task Regions[MaxDim][MaxDim];
process Coordinator_Task Coordinators[MaxDim][MaxDim];
process Protected_Counter Unfinished_Counter;
int RowRegions, ColRegions;
int NumRegions;
int RowsPerRegion, ColsPerRegion;

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
    printf("R,C,NumRegions = %d, %d, %d.\n",RowRegions,ColRegions,NumRegions);
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
    for (I=1; I<=RowRegions; I++)
	for (J=1; J<=ColRegions; J++) {
	    Regions[I][J] = create Region_Task(I,J);
	    Coordinators[I][J] = create Coordinator_Task();
	}
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
    Unfinished_Counter.WakeWhenZero();
    *p_DidNotConverge = DidNotConverge;
}


/*
 *  And now... the process bodies.
 */

process body Protected_Counter()
{
    int Counter;

    accept Initialize(Z) {
	Counter = Z;
    }
    for (;;)
	select {
	    accept Incr(Z) {
		Counter = Counter + Z;
		printf("after Incr, Counter is now %d.\n",Counter);
	    }
	or
	    accept Decr(Z) {
	        if (Counter != 0) /* this needs to be here to avoid time error */
		    Counter = Counter - Z;
		printf("after Decr, Counter is now %d.\n",Counter);
	    }
	or
	    accept Read() {
		treturn Counter;
	    }
	or (Counter == 0):
	    accept WakeWhenZero();
	or
	    terminate;
	}
}

process body Coordinator_Task()
{
    int Had_KeepOnGoing=0;
    int Had_Finish=0;

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
    Real New_Value;
    int MyRowLow, MyRowHi, MyColLow, MyColHi;
    int Iters,I,J;

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
printf("[%d][%d] MyRowLow=%d MyRowHi=%d MyColLow=%d MyColHi=%d.\n",
II,JJ,MyRowLow,MyRowHi,MyColLow,MyColHi);
    for (Iters=1; Iters<=MaxIterations; Iters++) {
	MyDone=1;
/*
 *	Compute a new value for each point in my region:
 */
	for (I=MyRowLow; I<=MyRowHi; I++) {
	    for (J=MyColLow; J<=MyColHi; J++) {
		New_Value = U[I][J] + Omega * 0.25 *
		    (U[I-1][J] + U[I][J-1] + U[I+1][J] + U[I][J+1] - 4.0*U[I][J]);

printf("Abs(New_Value - U[%d][%d])=%f, New_Value=%f, U[%d][%d]=%f.\n",I,J,
	Abs(New_Value - U[ I][ J]),    New_Value,I,J,U[ I][ J]);

		if (Abs(New_Value - U[I][J]) >= MaxErr)
		    MyDone = 0;
		U[I][J] = New_Value;
New_Value = U[I][J];
/*
		printf("MyDone=%d.\n",MyDone);

printf("U[%d][%d] := %f.\n",II,JJ,New_Value);
*/
	    }
	}
        if (!MyDone) {  /*Tell my neighbors to keep on going */
printf("[%d][%d]MyDone must be false, and it is %d.\n",II,JJ,MyDone);
	    if (MyRowRegion != 1)
		Coordinators[MyRowRegion-1][MyColRegion].KeepOnGoing();

	    if (MyRowRegion != RowRegions)
		Coordinators[MyRowRegion+1][MyColRegion].KeepOnGoing();

	    if (MyColRegion != 1)
		Coordinators[MyRowRegion][MyColRegion-1].KeepOnGoing();

	    if (MyColRegion != ColRegions)
		Coordinators[MyRowRegion][MyColRegion+1].KeepOnGoing();
	} else {
printf("[%d][%d]MyDone must be true, and it is %d.\n",II,JJ,MyDone);
	    Unfinished_Counter.Decr(1);
	    CurCount = Unfinished_Counter.Read();
	    if (CurCount == 0) {
		/* We're all done.  Wake up everybody who's sleeping: */
		All_Finish;
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
		if (CurCount == 0)
		    terminate;
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
}

main()
{
    int i,j,did_not_converge;
    static Real init_matrix[4][4] = {
	{1.0,	1.5,	2.0,	2.5},
	{1.5,	2.0,	2.5,	3.0},
	{2.0,	2.5,	3.0,	3.5},
	{2.5,	3.0,	3.5,	4.0} };
    shared Real matrix[4][4];

/*
 *  Shared arrays cannot be initialized, so we have to do it ourselves.
 */
    for (i=0; i<4; i++)
	for (j=0; j<4; j++)
	    matrix[i][j] = init_matrix[i][j];
    Parallel_Relaxation(matrix, 0.001, 1000, 16, &did_not_converge, 1.0, 3, 2);
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

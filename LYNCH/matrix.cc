pragma nolinesynch;
/*---------------------------------------------------------------------------
 * Do a matrix multiply C=AxB.  As a first cut, the algorithm is to assign
 * rows of A to processes, which read rows of B from shared memory.  This
 * should be a win because of snoopy cash (B is read-only).  C is kept in
 * a server which receives results from the dot products, which keeps C
 * out of shared memory (a lose from snoopy cache), and simplifies the end
 * condition detection. 
 *
 * A question left is whether or not A should be in shared memory, and let
 * the snoopy cache make the local copies, or should copies be explicitly
 * made in local memory.
 * 
 * William L. Lynch	Thu Jun 19 10:10:10 1986
 *---------------------------------------------------------------------------
 */



#include <stdio.h>
/*#define DEBUG 1*/



#define MAXpROCESSES 4

#define MAXrOWS 10
#define MAXcOLS 10


typedef float *rowArray[MAXrOWS];

rowArray A, B;





process spec C(int, int) { 			/* rows, cols in C */
	trans void answer(int, int, float);	/* row, col, answer */
};

 

process spec DotProd(int, int, int, int, process C);
						/* high and low rows
						   assigned from A, and
  						   size of B. */








/* C waits for answers from the dot product processes, and places them in
   the proper place in array C.  It also counts the number of results, for
   termination conditions. */
process body C(rows, cols)
{
	rowArray C;
	int i,j;

	shmallocMatrix(rows, cols, C);

	for (i=0; i < rows; i++) {
		for (j=0; j < cols; j++) {
			accept answer(row, col, num) {

#ifdef DEBUG
			printf("answer from %d, %d.\n",row,col);
#endif
				C[row][col] = num;
				treturn;
			}
		}
	}
	
	
	/* all answers have arrived */
	printMatrix(rows, cols, C);
}



/* dot product is initialized with a bank of rows from A, and then
   multiplies all by columns from B.  When a dot product is complete,
   process C is notified with the answer.  We are done when all rows and
   columns are used up */
process body DotProd(Ahigh, Alow, Brows, Bcols, C)
{
	int bc;	
	int i, j;
	float product;
	
	for (bc=0; bc < Bcols; bc++) {		/* for each column in B */
		for (i=Alow; i < Ahigh; i++) {	/* for each given A row */
			/* do a dot product */
			product = 0.0;
			for (j=0; j < Brows; j++) {
				product = A[i][j] * B[j][bc];
			}
			C.answer(i, bc, product);
		}
	}
}










shmallocMatrix(rows, cols, rowPtrs)
	int rows, cols;
	rowArray rowPtrs;
{
	int i;
	
	for (i=0; i < rows; i++) {
		rowPtrs[i] = (float *)shmalloc(sizeof(*rowPtrs[i])*cols);		
	}
}



printMatrix(rows, cols, matrix) 
	int rows, cols;
	rowArray matrix;
{
	int i,j;

	for (i=0; i < rows; i++) {
		for (j=0; j < cols; j++) {
			printf("%7.3f ", matrix[i][j]);
		}
	printf("\n");
	}
}



getMatrix(argc, argv, Arows, Acols, Brows, Bcols)
int argc;
char *argv[];
int *Arows, *Acols, *Brows, *Bcols;

{
	int i,j;
	/* for now, just generate some stuff. */
#define AROWS 10
#define ACOLS 9
#define BROWS 9
#define BCOLS 10	

	*Arows = AROWS;
	*Acols = ACOLS;
	*Brows = BROWS;
	*Bcols = BCOLS;
	
	shmallocMatrix(*Arows, *Acols, A);
	shmallocMatrix(*Brows, *Bcols, B);
	
	for (i=0; i < *Arows; i++) {
		for (j=0; j < *Acols; j++) {
			A[i][j] = i + j;
		}
	}
	
	for (i=0; i < *Brows; i++) {
		for (j=0; j < *Bcols; j++) {
			B[i][j] = i - j;
		}
	}

#ifdef DEBUG
	printf("A: \n"); printMatrix(*Arows, *Acols, A);
	printf("B: \n"); printMatrix(*Brows, *Bcols, B);
#endif
	
}










main (argc,argv)
int argc;
char *argv[];
{
	process C C;
	int Arows, Acols, Brows, Bcols;
	int Ahigh;
	int rowsPerProcess;
		
	getMatrix(argc, argv, &Arows, &Acols, &Brows, &Bcols);
	C = create C(Arows, Bcols);
	
	
	/* assign rows of A to processes, term on end is ceiling factor */
	rowsPerProcess = Arows / MAXpROCESSES + ((Arows % MAXpROCESSES) > 0);
	for (Ahigh=Arows; Ahigh > 0; Ahigh -= rowsPerProcess) {
		create DotProd(	Ahigh,
				Ahigh>=rowsPerProcess ? Ahigh-rowsPerProcess : 0,
				Brows, Bcols, C);
#ifdef DEBUG
		printf( "Started C process, Ahigh = %d.\n",Ahigh);
#endif		
	}
}


# 1 "matrix.cc"
pragma nolinesynch;



















# 1 "/usr/include/stdio.h"

















extern	struct	_iobuf {
	int	_cnt;
	char	*_ptr;
	char	*_base;
	int	_bufsiz;
	short	_flag;
	char	_file;
} _iob[20];




























struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*gets();
char	*fgets();
char	*sprintf();
# 21 "matrix.cc"










typedef float *rowArray[10];

rowArray A, B;





process spec C(int, int) { 			
	trans void answer(int, int, float);	
};

 

process spec DotProd(int, int, int, int, process C);
						













process body C(rows, cols)
{
	rowArray C;
	int i,j;

	shmallocMatrix(rows, cols, C);

	for (i=0; i < rows; i++) {
		for (j=0; j < cols; j++) {
			accept answer(row, col, num) {

# 73 "matrix.cc"

				C[row][col] = num;
				treturn;
			}
		}
	}
	
	
	
	printMatrix(rows, cols, C);
}







process body DotProd(Ahigh, Alow, Brows, Bcols, C)
{
	int bc;	
	int i, j;
	float product;
	
	for (bc=0; bc < Bcols; bc++) {		
		for (i=Alow; i < Ahigh; i++) {	
			
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
	





	*Arows = 10;
	*Acols = 9;
	*Brows = 9;
	*Bcols = 10	;
	
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

# 183 "matrix.cc"

	
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
	
	
	
	rowsPerProcess = Arows / 4 + ((Arows % 4) > 0);
	for (Ahigh=Arows; Ahigh > 0; Ahigh -= rowsPerProcess) {
		create DotProd(	Ahigh,
				Ahigh>=rowsPerProcess ? Ahigh-rowsPerProcess : 0,
				Brows, Bcols, C);
# 217 "matrix.cc"

	}
}


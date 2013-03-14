/* Name: Pradeep Kumar Vikraman
 * ID: pvikrama@andrew.cmu.edu
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"
#include "contracts.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. The REQUIRES and ENSURES from 15-122 are included
 *     for your convenience. They can be removed if you like.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    REQUIRES(M > 0);
    REQUIRES(N > 0);

    ENSURES(is_transpose(M, N, A, B));

    int i,j,k,l,var1,var2,var3,var4,var5,var6,var7,var8;

    REQUIRES(M > 0);
    REQUIRES(N > 0);

    /*I have chosen a blocksize of 8*8 because of 2 reasons:
	 *1)The cache block size is 8 
	 *2)A group of successive 8*32 lines of either A or B 
	 *can be stored at the same time in cache without conflict 
	 */
	if(M==32 && N==32)
	{
		for (i = 0; i < N; i += 8) 	   
			for (j = 0; j < M; j += 8) 
				if(i==j && (i % 8)==0)
				{
					for (k = i; k<i+8; ++k)
						for (l = j; l<j+8 ;++l)
							if(l!=k)
								B[l][k] = A[k][l];
							//Avoids diagonal misses by storing in variables
							else if(l==k && l==i)
								var1=A[l][l];
							else if(l==k && l==i+1)
								var2=A[l][l];
							else if(l==k && l==i+2)
								var3=A[l][l];
							else if(l==k && l==i+3)
								var4=A[l][l];
							else if(l==k && l==i+4)
								var5=A[l][l];
							else if(l==k && l==i+5)
								var6=A[l][l];
							else if(l==k && l==i+6)
								var7=A[l][l];
							else if(l==k && l==i+7)
								var8=A[l][l];
					for (k = i; k<i+8; ++k)
						for (l = j; l<j+8 ;++l)
							if(l==k && l==i)
								B[l][l]=var1;
							else if(l==k && l==i+1)
								B[l][l]=var2;
							else if(l==k && l==i+2)
								B[l][l]=var3;
							else if(l==k && l==i+3)
								B[l][l]=var4;
							else if(l==k && l==i+4)
								B[l][l]=var5;
							else if(l==k && l==i+5)
								B[l][l]=var6;
							else if(l==k && l==i+6)
								B[l][l]=var7;
							else if(l==k && l==i+7)
								B[l][l]=var8;
					continue;
				}
				else
				{
					for (k = i; k<i+8; ++k)
						for (l = j; l <j+8 ;++l)
							B[l][k] = A[k][l];
				}
	}

	/*I have chosen a blocsize of 4*4 due to:
	 *1)A group of successive 4*32 lines of either A or B 
	 *can be stored in cache without conflict
	 *2)Hence,even though block size is 8, i could utilize only 4 
	 *at a time
	 */
	if(M==64 && N==64)
	{
		int bsize=4;
		for (i = 0; i < N; i += bsize)
			for (j = 0; j < M; j += bsize) 			
				//Avoids diagonal misses by storing in variables
				if(i==j && (i % bsize)==0)
				{
					for (k = i; k<i+bsize; ++k)
						for (l = j+(bsize-1); l>=j ;--l)
							if(l!=k)
								B[l][k] = A[k][l];
							else if(l==k && l==i)
								var8=A[l][l];
							else if(l==k && l==i+1)
								var1=A[l][l];
							else if(l==k && l==i+2)
								var2=A[l][l];
							else if(l==k && l==i+3)
								var3=A[l][l];
					for (k = i; k<i+bsize; ++k)
						for (l = j+(bsize-1); l>=j ;--l)
							if(l==k && l==i)
								B[l][l]=var8;
							else if(l==k && l==i+1)
								B[l][l]=var1;
							else if(l==k && l==i+2)
								B[l][l]=var2;
							else if(l==k && l==i+3)
								B[l][l]=var3;
					continue;
				}
				else
				{	
					for (k = i; k<i+bsize; ++k)
						for (l = j+(bsize-1); l>=j ;--l)
							if(k<64 && l<64)
							{
							B[k][l] = A[l][k];
							}
				}
	}
	
	/*I have chosen a block size of 14 because this gave me the best
	 *performance when I tried different block sizes 
	 */
	if(M==61 && N==67)
    {
		int bsize=14;
		for(i=0;i<N;i=i+bsize)
			for(j=0;j<M;j=j+bsize)
				for(k=i;k<i+bsize;k++)
					for(l=j;l<j+bsize;l++)
						if(k<67 && l<61)
							B[l][k]=A[k][l];
	}
}
/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j,tmp;

    REQUIRES(M > 0);
    REQUIRES(N > 0);

  for (i = 0; i < N; i++) {
       for (j = 0; j < M; j++) {
           tmp = A[i][j];
           B[j][i] = tmp;
        }
    }
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
	/* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
			if (A[i][j] != B[j][i]) {
				return 0;
			}
		}
    }
    return 1;
}


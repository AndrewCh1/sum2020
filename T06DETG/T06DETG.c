# include <stdio.h>
# include <windows.h>
typedef DOUBLE DBL;
DBL A[MAX][MAX];
INT N;

BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;

  N = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  fscanf(F, "%d", &N);
  if (N > MAX)
    N = MAX;
  else
    if (N < 0)
      N = 0;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);

  fclose(F);
  return TRUE;
}

VOID Go( VOID )
{
  INT i, j, max_row, max_col, x, y;
  DBL det = 1;

  for (i = 0; i < N; i++)
  {
    max_row = max_col = i;
    for (y = i; y < N; y++)
     for (x = i; x < N; x++)
        if (fabs(A[y][x]) > fabs(A[max_row][max_col]))
         max_col = x, max_row = y;
    if (A[max_row][max_col] == 0)
    {
    det = 0;
    break;
    }

    if (max_row != i)
    {
    /* Swap max_row row and i row (elements [i..N-1]) */
      for (x = i; x < N; x++)
        Swap(&A[i][x], &A[max_row][x]);
      det = -det;
    }
    if (max_col != i)
    {
     /* Swap max_col column and i column (elements [0..N-1]) */
       for (x = 0; x < N; x++)
         Swap(&A[x][i], &A[x][max_col]);
      det = -det;
    }
    
    coef = A[j][i] / A[i][i];
    A[j][i] = 0;
    for(k = i + 1, k < N, k++)
      A[j][k] -= A[i][k] * coef;
    det *= A[i][i];
}



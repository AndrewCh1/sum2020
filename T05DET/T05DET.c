#include <stdio.h>
#include <windows.h>
#define MAX 10
  typedef DOUBLE DBL;
  DBL A[MAX][MAX];
  INT N;

  INT P[MAX];
  BOOL IsParity;
  DBL Det = 0;

BOOL LoadMatrix( CHAR *FileName );
VOID Go( INT Pos );
VOID Swap ( DBL *a, DBL *b )
{
  INT temp = *a;
  *a = *b; 
  *b = temp;
}


VOID main (VOID)
{
  LoadMatrix("F.txt");
  Go(0);
  printf("%lf", Det);
  getch();
}

BOOL LoadMatrix( CHAR *FileName )
{
  INT i,j;
  FILE *F;

  N = 3;
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

VOID Go( INT Pos )
{
  INT i, j; 
  DBL prod;

  if (Pos == N)
  {
    for (prod = 1, i = 0; i < N; i++)
      prod *= A[i][P[i]];
    Det += (IsParity ? 1 : -1) * prod;
  }
  else
  {
    for (j = Pos; j < N; j++)
    {
      Swap(&P[Pos], &P[j]);
      if(Pos != j)
        IsParity = !IsParity;
      Go(Pos + 1);
      Swap(&P[Pos], &P[j]);
      if(Pos != j)
        IsParity = !IsParity;
    }
  }
}

/* Chuvatkin Andrey, 10-6, 02.06.2020*/
#include <stdio.h>
#include <math.h>
#include <windows.h>

#define MAX 5
INT P[MAX];
BOOL IsParity;


VOID swap(INT *a, INT *b) 
{
  int temp = *a;
  *a = *b; 
  *b = temp;
}

VOID store ( VOID )
{
  FILE *F;
  INT i;

  F = fopen("File.txt", "a");
  if (F == NULL)
    return;

  for( i = 0; i < MAX - 1; i++)
    fprintf(F, "%d", P[i]);
  fprintf(F, "%d – parity: %s\n", P[MAX - 1], IsParity ? "even" : "odd");

  fclose(F);
}

VOID Go( INT Pos )
{
  INT i;
  if (Pos == MAX)
    store();
  else
  {
    for (i = Pos; i < MAX; i++)
    {
      swap(&P[Pos], &P[i]);
      if (Pos != i)
        IsParity = !IsParity;
      store();
      Go(Pos + 1);
      swap(&P[Pos], &P[i]);
    }
  }
}

VOID main ( VOID )
{
  INT i;
  for (i = 0; i < MAX; i++)
    P[i] = i + 1;
  Go(0);
}

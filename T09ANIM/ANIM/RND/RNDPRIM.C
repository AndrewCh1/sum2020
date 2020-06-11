#include <stdio.h>
#include <string.h>

#include "rnd.h"

/* The primitive drawing function.
 * ARGUMENTS:
 *   - Primitive adress:
 *       ac6PRIM *Pr;
 *   - Number of points:
 *       INT NoofV;
 *   - Number of indeces:
 *       INT NoofI;
 * RETURNS:
 *   TRUE or FALSE.
 */
BOOL AC6_RndPrimCreate( ac6PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  memset(Pr, 0, sizeof(ac6PRIM));
  size = sizeof(ac6VERTEX) * NoofV + sizeof(INT) * NoofI;

  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);

  return TRUE;
}

BOOL AC6_RndPrimCreateSphere(ac6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
{
  INT i, j, k = 0;
  DBL phi, theta;

  if (!AC6_RndPrimCreate(Pr, SplitW * SplitH, 2 * 3 * (SplitW - 1) * (SplitH - 1)))
    return FALSE;

  for (i = 0, theta = 0; i < SplitH; i++, theta += PI / (SplitH - 1))
    for (j = 0, phi = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
      Pr->V[k++].P = VecSet(C.X + R * sin(theta) * sin(phi), C.Y + R * cos(theta), C.Z + R * sin(theta) * cos(phi));
  k = 0;
  for (i = 0; i < SplitH - 1; i++)
    for (j = 0; j < SplitW - 1; j++)
    {
      Pr->I[k++] = i * SplitW + j;
      Pr->I[k++] = (i + 1) * SplitW + j;
      Pr->I[k++] = i * SplitW + j + 1;

      Pr->I[k++] = (i + 1) * SplitW + j;
      Pr->I[k++] = i * SplitW + j + 1;
      Pr->I[k++] = (i + 1) * SplitW + j + 1;
    }

  return TRUE;
}


VOID AC6_RndPrimFree( ac6PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(ac6PRIM));
}

/* Primitive loading*/
BOOL AC6_RndPrimLoad( ac6PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0;
  static CHAR Buf[1000];

  memset(Pr, 0, sizeof(ac6PRIM));

  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Vertex and factes count */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;

  if (!AC6_RndPrimCreate(Pr, nv, nf * 3))
  {
    fclose(F);
    return FALSE;
  }

  /* Load geometry data */
  rewind(F);
  nv = 0;
  nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf %lf %lf", &x, &y, &z);
      Pr->V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3) == 3;
      Pr->I[nf++] = n1 - 1;
      Pr->I[nf++] = n2 - 1;
      Pr->I[nf++] = n3 - 1;
    }


  fclose(F);
  return TRUE;
} /* End of 'VG4_RndPrimCreateCone' function */


VOID AC6_RndPrimDraw( ac6PRIM *Pr, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr3(Pr->Trans, World, AC6_RndMatrVP);
  POINT *pnts;

  if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;

  /* Build projection */
  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, wvp);

    pnts[i].x = (LONG)((p.X + 1) * AC6_RndFrameW / 2);
    pnts[i].y = (LONG)((-p.Y + 1) * AC6_RndFrameH / 2);
  }

  /* Draw triangles */
  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(AC6_hRndDC, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL);
    LineTo(AC6_hRndDC, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(AC6_hRndDC, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(AC6_hRndDC, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);
}/* End of drawing function */






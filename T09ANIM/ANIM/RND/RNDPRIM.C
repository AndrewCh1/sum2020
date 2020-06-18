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
VOID AC6_RndPrimCreate( ac6PRIM *Pr, ac6VERTEX *V, INT *I, INT NumOfV, INT NumOfI, ac6PRIM_TYPE Type )
{
  memset(Pr, 0, sizeof(ac6PRIM));

  Pr->Type = Type;
  if (V != NULL)
  {
    /*Genrate buffers*/
    glGenBuffers(1, &Pr->VBuf);
    glGenVertexArrays(1, &Pr->VA);

    glBindVertexArray(Pr->VA);

    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
   

    glBufferData(GL_ARRAY_BUFFER, sizeof(ac6VERTEX) * NumOfV, V, GL_STATIC_DRAW);
   

    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(ac6VERTEX),
                        (VOID *)0); /* позиция */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(ac6VERTEX),
                        (VOID *)sizeof(VEC)); /* текстурные координаты */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(ac6VERTEX),
                        (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* нормаль */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(ac6VERTEX),
                        (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* цвет */

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
  }
  if(I != NULL)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
    Pr->NumOfElements = NumOfI;
  }
  else
    Pr->NumOfElements = NumOfV;
  Pr->Trans = MatrIdentity();
}

BOOL AC6_RndPrimCreateSphere(ac6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
{
  INT i, j, k = 0, noofv, size;
  DBL phi, theta;
  ac6VERTEX *V;
  INT *Ind;
  VEC L = VecNormalize(VecSet(1, 1, 1));

  memset(Pr, 0, sizeof(ac6PRIM));
  noofv = SplitW * SplitH;

  size = sizeof(ac6VERTEX) * noofv;

  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + noofv);

  for (i = 0, theta = 0; i < SplitH; i++, theta += PI / (SplitH - 1))
    for (j = 0, phi = 0; j < SplitW; j++, phi += 2 * PI / (SplitW - 1))
    {
      FLT
        x = sin(theta) * sin(phi),
        y = cos(theta),
        z = sin(theta) * cos(phi),
        nl = VecDotVec(VecSet(x, y, z), L);
 
      V[k].P = VecSet(C.X + R * x, C.Y + R * y, C.Z + R * z);
      if (nl < 0.1)
        nl = 0.1;
      V[k].C = Vec4Set(nl, nl, nl, 1);
      k++;
    }
  k = 0;
  
  AC6_RndPrimCreateFromGrid(Pr, V, SplitW, SplitH, FALSE );

  return TRUE;
}


VOID AC6_RndPrimFree( ac6PRIM *Pr )
{
  if (Pr->VA != 0)
  {
    /* делаем активным массив вершин */
    glBindVertexArray(Pr->VA);
    /* "отцепляем" буфер */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Pr->VBuf);
    /* делаем неактивным массив вершин */
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Pr->VA);
  }
  if (Pr->IBuf != 0)
    glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(ac6PRIM));
}

/*Primitive creating from grid*/
BOOL AC6_RndPrimCreateFromGrid( ac6PRIM *Pr, ac6VERTEX *V, INT W, INT H, BOOL IsNormalNeed )
{
  INT i, j, k;
  INT *Ind;

  memset(Pr, 0, sizeof(ac6PRIM));
  if ((Ind = malloc(((2 * W + 1) * (H - 1) - 1) * sizeof(INT))) == NULL)
    return FALSE;

  for (i = 0, k = 0; i < H - 1; i++)
  {
    for (j = 0; j < W; j++)
    {
      Ind[k++] = (i + 1) * W + j;
      Ind[k++] = i * W + j;
    }
    if (i != H - 2)
      Ind[k++] = -1;
  }

  if(IsNormalNeed)
  {
   for (i = 0; i < W * H; i++)
     V[i].N = VecSet(0, 0, 0);

   for (i = 0; i < H - 1; i++)
    for (j = 0; j < W - 1; j++)
    {
      ac6VERTEX
        *P00 = V + i * W + j,
        *P01 = V + i * W + j + 1,
        *P10 = V + (i + 1) * W + j,
        *P11 = V + (i + 1) * W + j + 1;
      VEC N;

      N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                   VecSubVec(P11->P, P10->P)));
      P00->N = VecAddVec(P00->N, N);
      P10->N = VecAddVec(P10->N, N);
      P11->N = VecAddVec(P11->N, N);

      N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                   VecSubVec(P00->P, P01->P)));
      P00->N = VecAddVec(P00->N, N);
      P01->N = VecAddVec(P01->N, N);
      P11->N = VecAddVec(P11->N, N);
    }
    for (i = 0; i < W * H; i++)
      V[i].N = VecNormalize(V[i].N);
 
  }

  AC6_RndPrimCreate(Pr, V, Ind, W * H, 
    (2 * W + 1) * (H - 1) - 1,  AC6_RND_PRIM_TRISTRIP);
  free(Ind);
  return TRUE;
} /* End of AC6_RndPrimCreateFromGrid function */


/* Primitive loading*/
BOOL AC6_RndPrimLoad( ac6PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0, size;
  ac6VERTEX *V;
  INT *Ind;
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

  size = sizeof(ac6VERTEX) * nv + sizeof(INT) * nf * 3;

  if ((V = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Ind = (INT *)(V + nv);
  memset(V, 0, size);

  /* Load geometry data */
  rewind(F);
  nv = 0;
  nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf %lf %lf", &x, &y, &z);
      V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3) == 3;
      Ind[nf++] = n1 - 1;
      Ind[nf++] = n2 - 1;
      Ind[nf++] = n3 - 1;
    }

  AC6_RndPrimCreate(Pr, V, Ind, nv, nf, AC6_RND_PRIM_TRIMESH);

  fclose(F);
  return TRUE;
} /* End of 'AC6_RndPrimCreateCone' function */


VOID AC6_RndPrimDraw( ac6PRIM *Pr, MATR World )
{
  INT loc, ProgId;
  MATR
    w = MatrMulMatr2(Pr->Trans, World),
    winv = MatrTranspose(MatrInverse(w)),
    wvp = MatrMulMatr2(w, AC6_RndMatrVP);

  INT gl_prim_type = Pr->Type == AC6_RND_PRIM_LINES ? GL_LINES :
                     Pr->Type == AC6_RND_PRIM_TRIMESH ? GL_TRIANGLES :
                     Pr->Type == AC6_RND_PRIM_TRISTRIP ? GL_TRIANGLE_STRIP :
                     GL_POINTS;


  glLoadMatrixf(wvp.A[0]);

  ProgId = AC6_RndMtlApply(Pr->MtlNo);

  glUseProgram(ProgId);
  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, AC6_Anim.Time);
  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrW")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, w.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrWInv")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, winv.A[0]);

  /*Camera*/
  if ((loc = glGetUniformLocation(ProgId, "CamLoc")) != -1)
    glUniform3fv(loc, 1, &AC6_RndCamLoc.X);
  if ((loc = glGetUniformLocation(ProgId, "CamRight")) != -1)
    glUniform3fv(loc, 1, &AC6_RndCamLoc.X);
  if ((loc = glGetUniformLocation(ProgId, "CamUp")) != -1)
    glUniform3fv(loc, 1, &AC6_RndCamLoc.X);
  if ((loc = glGetUniformLocation(ProgId, "CamDir")) != -1)
    glUniform3fv(loc, 1, &AC6_RndCamLoc.X);

  if (Pr ->IBuf != 0)
  {
    /* делаем активным массив вершин */
    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    /* отрисовка */
    glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  else
  {
  /* выключили массив вершин */
    glBindVertexArray(Pr->VA);
    glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
    glBindVertexArray(0);
  }

}/* End of drawing function */






/* FILE NAME: MTH.H
 * PROGRAMMER: AC6
 * DATE: 05.06.2020
 * PURPOSE: 3D math 
 */
#ifndef __MTH_H_
#define __MTH_H_

#include <math.h>
#include <windows.h>

#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

/*Useful macros*/
#define MatrMulMatr2(A, B) MatrMulMatr(A, B)
#define MatrMulMatr3(A, B, C) MatrMulMatr(MatrMulMatr(A, B), C)


typedef DOUBLE DBL;
typedef FLOAT FLT;

typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

typedef struct tagVEC2
{
  FLT X, Y;
} VEC2;

typedef struct tagVEC4
{
  FLT X, Y, Z, W;
} VEC4;

typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */

__inline VEC VecSet1( FLT X )
{
  VEC v;
  v.X = X;
  v.Y = X;
  v.Z = X;
  return v;
}

__inline VEC4 Vec4Set( FLT A, FLT B, FLT C, FLT D )
{
  VEC4 v;

  v.X = A;
  v.Y = B;
  v.Z = C;
  v.W = D;
  return v;
}
__inline VEC4 Vec4Set1( FLT A )
{
  VEC4 v;
  v.X = A;
  v.Y = A;
  v.Z = A;
  v.W = A;
  return v;
}


__inline VEC2 Vec2Set( FLT A, FLT B )
{
  VEC2 v;
  v.X = A;
  v.Y = B;
  return v;
}

__inline VEC2 Vec2Set1( FLT A )
{
  VEC2 v;
  v.X = A;
  v.Y = A;
  return v;
}

/*Adding vec*/
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  VEC v;

  v.X = V1.X + V2.X;
  v.Y = V1.Y + V2.Y;
  v.Z = V1.Z + V2.Z;
  return v;
}

/*Substrackt vec*/
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  VEC v;

  v.X = V1.X - V2.X;
  v.Y = V1.Y - V2.Y;
  v.Z = V1.Z - V2.Z;
  return v;
}

/*Number multiply*/
__inline VEC VecMulNum( VEC V1, FLT N )
{
  VEC v;

  v.X = V1.X * N;
  v.Y = V1.Y * N;
  v.Z = V1.Z * N;
  return v;
}

/*Number division*/
__inline VEC VecDivNum( VEC V1, FLT N )
{
  VEC v;

  v.X = V1.X / N;
  v.Y = V1.Y / N;
  v.Z = V1.Z / N;
  return v;
}

/*Negative vec*/
__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
}

/*DotVec*/
__inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

/*CrossVec*/
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X);
}


/*Vec lengh*/
__inline FLT VecLen( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
}

/*Vec Normalize*/
__inline VEC VecNormalize( VEC V )
{
  FLT len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return V;
  len = sqrt(len);
  return VecDivNum(V, len);
}

/*Point transformation*/
__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]),
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]),
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]));
}


__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(
                (V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0]),
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1]),
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2])
               );
}


__inline VEC VecMulMatr( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];
  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}


__inline VEC VecMulMatr4x4( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.X * M.A[1][3] + V.X * M.A[2][3] + V.X * M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}

/*MATRIX REALISATION*/

static MATR UNIT_MATRIX =
{                  \
  {                \
    {1, 0, 0, 0},  \
    {0, 1, 0, 0},  \
    {0, 0, 1, 0},  \
    {0, 0, 0, 1}   \
  }                \
};

__inline MATR MatrIdentity()
{
  return UNIT_MATRIX;
} 

__inline MATR MatrTranslate( VEC T )
{
  MATR m = UNIT_MATRIX;

  m.A[3][0] = T.X;
  m.A[3][1] = T.Y;
  m.A[3][2] = T.Z;
  return m;
}


__inline MATR MatrScale( VEC T )
{
  MATR m = UNIT_MATRIX;

  m.A[0][0] = T.X;
  m.A[1][1] = T.Y;
  m.A[2][2] = T.Z;
  return m;
}


/*rotation maxrixes*/
__inline MATR MatrRotateX( DBL AngleInDegree )
{
  FLT
    a = D2R(AngleInDegree),
    c = cos(a),
    s = sin(a);
  MATR m = UNIT_MATRIX;
  m.A[1][1] = c;
  m.A[1][2] = s;
  m.A[2][1] = -s;
  m.A[2][2] = c;
  return m;
}


__inline MATR MatrRotateY( DBL AngleInDegree )
{
  FLT
    a = D2R(AngleInDegree),
    c = cos(a),
    s = sin(a);
  MATR m = UNIT_MATRIX;
  m.A[0][0] = c;
  m.A[0][2] = -s;
  m.A[2][0] = s;
  m.A[2][2] = c;
  return m;
}


__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  FLT
    a = D2R(AngleInDegree),
    c = cos(a),
    s = sin(a);
  MATR m = UNIT_MATRIX;
  m.A[0][0] = c;
  m.A[0][2] = -s;
  m.A[2][0] = s;
  m.A[2][2] = c;
  return m;
}


__inline MATR MatrRotate( DBL AngleInDegree, VEC V )
{
  FLT
    a = D2R(AngleInDegree),
    c = cos(a),
    s = sin(a);
  VEC A = VecNormalize(V);
  MATR m =
  {
    {
      {c + A.X * A.X * (1 - c), A.X * A.Y * (1 - c) + A.Z * s, A.X * A.Z * (1 - c) - A.Y * s, 0},
      {A.Y * A.X * (1 - c) - A.Z * s, c + A.Y * A.Y * (1 - c), A.Y * A.Z * (1 - c) + A.X * s, 0},
      {A.Z * A.X * (1 - c) + A.Y * s, A.Z * A.Y * (1 - c) - A.X * s, c + A.Z * A.Z * (1 - c), 0},
      {0, 0, 0, 1}
    }
  };

  return m;
}


/*Multiply matrixes*/
__inline MATR MatrMulMatr( MATR M1, MATR M2 ) 
{
  INT i, j, k;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.A[i][j] = 0, k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
}

/*3x3 Matrix determinator*/
__inline FLT MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 + 
         A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31;
}

/*Any matrix determinator*/
__inline FLT MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) +
    -M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                               M.A[2][0], M.A[2][2], M.A[2][3],
                               M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) +
    -M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                               M.A[2][0], M.A[2][1], M.A[2][2],
                               M.A[3][0], M.A[3][1], M.A[3][2]);
}
__inline MATR MatrInverse( MATR M )
{
  MATR r;
  FLT det = MatrDeterm(M);

  if (det == 0)
  {
    r = UNIT_MATRIX;
    return r;
  }

  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;


  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][3] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2], 
                   M.A[2][0], M.A[2][1], M.A[2][2]) / det;

  return r;
}

__inline MATR MatrTranspose(MATR M)
{
MATR M1;

M1.A[0][0] = M.A[0][0];
M1.A[0][1] = M.A[1][0];
M1.A[0][2] = M.A[2][0];
M1.A[0][3] = M.A[3][0];
M1.A[1][0] = M.A[0][1];
M1.A[1][1] = M.A[1][1];
M1.A[1][2] = M.A[2][1];
M1.A[1][3] = M.A[3][1];
M1.A[2][0] = M.A[0][2];
M1.A[2][1] = M.A[1][2];
M1.A[2][2] = M.A[2][2];
M1.A[2][3] = M.A[3][2];
M1.A[3][0] = M.A[0][3];
M1.A[3][1] = M.A[1][3];
M1.A[3][2] = M.A[2][3];
M1.A[3][3] = M.A[3][3];

return M1;
}

/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC 
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));

  MATR m =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };
  return m;
}  /* End of 'MatrView' function */

__inline MATR MatrOrtho(FLT l, FLT r, FLT b, FLT t, FLT n, FLT f)
{
  MATR m =
  {
   {
    {2 / (r - l), 0, 0, 0},
    {0, 2 / (t - b), 0, 0},
    {0, 0, -(2 / (f - n)), 0},
    {-(r + l) / (r - l), -(t + b) / (t - b), -(( n + f) / (f - n)), 1}
   }
  };
  return m;
}

__inline MATR MatrFrustum(FLT l, FLT r, FLT b, FLT t, FLT n, FLT f)
{
  MATR m =
  {
   {
    {(2 * n) / (r - l), 0, 0, 0},
    {0, (2 * n) / (t - b), 0, 0},
    {(r + l) / (r - l), (t + b) / (t - b), -((f + n) / (f - n)), -1},
    {0, 0, -((2 * n * f) / (f - n)), 0}
   }
  };
  return m;
}

#endif

/* END OF 'MTH.H' FILE */

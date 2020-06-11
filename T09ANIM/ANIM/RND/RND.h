/**/
#ifndef __RND_H_
#define __RND_H_

#define GLEW_STATIC

#include "glew.h"
#include <gl/gl.h>


#include "../../def.h"

extern HWND AC6_hRndWnd;        /* Work window handle */
extern HDC AC6_hRndDC;     /* Work window memory device context */
extern INT AC6_RndFrameW, AC6_RndFrameH, BmFrame; /* Work window size */
extern HGLRC AC6_hRndGLRC;

extern DBL
  AC6_RndProjSize,  /* Project plane fit square */
  AC6_RndProjDist,  /* Distance to project plane from viewer (near) */
  AC6_RndProjFarClip;  /* Distance to project far clip plane (far) */
MATR
  AC6_RndMatrView, /* View coordinate system matrix */
  AC6_RndMatrProj, /* Projection coordinate system matrix */
  AC6_RndMatrVP;   /* Stored (View * Proj) matrix */

VOID AC6_RndInit( HWND hWnd );
VOID AC6_RndClose( VOID );
VOID AC6_RndResize( INT W, INT H );
VOID AC6_RndProjSet( VOID );
VOID AC6_RndStart( VOID );
VOID AC6_RndEnd( VOID );
VOID AC6_RndCopyFrame( VOID );
VOID AC6_RndCamSet( VEC Loc, VEC At, VEC Up );


/***
 * Primitive handle
 ***/

/**/
typedef struct tagac6VERTEX
{
  VEC P;
} ac6VERTEX;

/* FILE NAME: RND.H
 * PROGRAMMER: AC6
 * DATE: 05.06.2020
 * PURPOSE: 3D animation project.
 *          declaration module.
 */
typedef struct tagac6PRIM
{
  ac6VERTEX *V; /* Vertex attributes array */
  INT NumOfV;   /* Number of vertices */

  INT *I;       /* Index array (for trimesh – by 3 ones) */
  INT NumOfI;   /* Number of indices */

  MATR Trans;   /* Additional transformation matrix */
} ac6PRIM;

/*Primitive drawing functions*/
BOOL AC6_RndPrimCreate( ac6PRIM *Pr, INT NoofV, INT NoofI );
VOID AC6_RndPrimFree( ac6PRIM *Pr );
VOID AC6_RndPrimDraw( ac6PRIM *Pr, MATR World );
BOOL AC6_RndPrimCreateSphere( ac6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH );
BOOL AC6_RndPrimLoad( ac6PRIM *Pr, CHAR *FileName );


#endif
/* End of 'RND.h' file */

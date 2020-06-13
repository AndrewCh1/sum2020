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
extern INT AC6_RndProgId;

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
  VEC P;   /* позици€ */
  VEC2 T;  /* текстурна€ координата */
  VEC N;   /* нормаль */
  VEC4 C;  /* ÷вет (r,g,b,a) */
} ac6VERTEX;


/* FILE NAME: RND.H
 * PROGRAMMER: AC6
 * DATE: 05.06.2020
 * PURPOSE: 3D animation project.
 *          declaration module.
 */

typedef enum tagac6PRIM_TYPE
{
  AC6_RND_PRIM_TRIMESH,  /* Triangle mesh - array of triangles */
  AC6_RND_PRIM_TRISTRIP, /* Triangle strip - array of stripped triangles */
  AC6_RND_PRIM_LINES,    /* Line segments (by 2 points) */
  AC6_RND_PRIM_POINTS,   /* Arrauy of points */
} ac6PRIM_TYPE;

typedef struct tagac6PRIM
{
  ac6PRIM_TYPE Type;
  INT
    VA, VBuf, IBuf;       /* Index array (for trimesh Ц by 3 ones) */
  INT NumOfElements;

  MATR Trans;   /* Additional transformation matrix */
} ac6PRIM;




/*Primitive drawing functions*/
VOID AC6_RndPrimCreate( ac6PRIM *Pr, ac6VERTEX *V, INT *I, INT NumOfV, INT NumOfI, ac6PRIM_TYPE Type );
VOID AC6_RndPrimFree( ac6PRIM *Pr );
VOID AC6_RndPrimDraw( ac6PRIM *Pr, MATR World );
BOOL AC6_RndPrimCreateSphere( ac6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH );
BOOL AC6_RndPrimLoad( ac6PRIM *Pr, CHAR *FileName );

/***
 * Shader handle
 ***/

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader program Id.
 */
INT AC6_RndShdLoad( CHAR *ShaderFileNamePrefix );

/* Unload shader program from memory files function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETURNS: None.
 */
VOID AC6_RndShdDelete( INT ProgId );


#endif
/* End of 'RND.h' file */

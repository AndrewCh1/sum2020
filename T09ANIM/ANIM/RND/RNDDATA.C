#include "rnd.h"

HWND AC6_hRndWnd;        /* Work window handle */
HDC AC6_hRndDC;     /* Work window memory device context  */
INT AC6_RndFrameW, AC6_RndFrameH, BmFrame; /* Work window size */
HGLRC AC6_hRndGLRC;
INT AC6_RndProgId;

DBL
  AC6_RndProjSize,  /* Project plane fit square */
  AC6_RndProjDist,  /* Distance to project plane from viewer (near) */
  AC6_RndProjFarClip;  /* Distance to project far clip plane (far) */
MATR
  AC6_RndMatrView, /* View coordinate system matrix */
  AC6_RndMatrProj, /* Projection coordinate system matrix */
  AC6_RndMatrVP;   /* Stored (View * Proj) matrix */

VEC
  AC6_RndCamLoc,
  AC6_RndCamAt,
  AC6_RndCamDir,
  AC6_RndCamUp,
  AC6_RndCamRight;
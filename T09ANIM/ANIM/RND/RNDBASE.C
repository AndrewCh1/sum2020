/**/

#pragma comment(lib, "opengl32")

#include <time.h>

#include "rnd.h"
#include "resourses/rndres.h"

/**/
VOID AC6_RndInit( HWND hWnd )
{ 
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};


  AC6_hRndWnd = hWnd;
  AC6_hRndDC = GetDC(AC6_hRndWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;

  i = ChoosePixelFormat(AC6_hRndDC, &pfd);
  DescribePixelFormat(AC6_hRndDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(AC6_hRndDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  AC6_hRndGLRC = wglCreateContext(AC6_hRndDC);
  wglMakeCurrent(AC6_hRndDC, AC6_hRndGLRC);

  /* Initialize GLEW library */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    MessageBox(hWnd, "Error OpenGL initialization", "ERROR",
      MB_ICONERROR | MB_OK);
    exit(0);
  }

  /*set def. parameters*/
  glClearColor(0.30, 0.50, 0.8, 1);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  AC6_RndFrameH = AC6_RndFrameW = 47;

  AC6_RndProjSize = AC6_RndProjDist = 0.1;  /* Project plane fit square */
  AC6_RndProjFarClip = 3000;

  AC6_RndProjSet();
  AC6_RndCamSet(VecSet(3, 3, 3), VecSet(0, 0 ,0), VecSet(0, 1, 0));

  /// AC6_RndProgId = AC6_RndShdLoad("DEFAULT");
}

VOID AC6_RndClose( VOID )
{
  /// AC6_RndShdDelete(AC6_RndProgId);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AC6_hRndGLRC);
  ReleaseDC(AC6_hRndWnd, AC6_hRndDC);
} /* End of 'AC6_RndClose' function */
  
VOID AC6_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);

  /* сохраняем размеры */
  AC6_RndFrameW = W;
  AC6_RndFrameH = H;

  /* пересчитываем проекию */
  AC6_RndProjSet();

 }

VOID AC6_RndCopyFrame( VOID )
{
   /// SwapBuffers(VG4_Anim.hDC);
  wglSwapLayerBuffers(AC6_hRndDC, WGL_SWAP_MAIN_PLANE);
}

VOID AC6_RndStart(VOID)
{
#ifndef NDEBUG
  INT t = clock();
  static INT reload_time;

  if (t - reload_time > CLOCKS_PER_SEC)
  {
    AC6_RndShdUpdate();
    reload_time = t;
  }
#endif

  AC6_RndLightShadow();

  /*Clear Background*/
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

VOID AC6_RndEnd( VOID )
{
  glFinish();
}

VOID AC6_RndProjSet( VOID )
{
  DBL rx, ry;

  rx = ry = AC6_RndProjSize;

 /* Correct aspect ration */
  if (AC6_RndFrameW > AC6_RndFrameH)
    rx *= (DBL)AC6_RndFrameW / AC6_RndFrameH;
  else
    ry *= (DBL)AC6_RndFrameH / AC6_RndFrameW;


  AC6_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      AC6_RndProjDist, AC6_RndProjFarClip);
  AC6_RndMatrVP = MatrMulMatr(AC6_RndMatrView, AC6_RndMatrProj);
}

VOID AC6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  AC6_RndMatrView = MatrView(Loc, At, Up);
  AC6_RndMatrVP = MatrMulMatr(AC6_RndMatrView, AC6_RndMatrProj);

  AC6_RndCamLoc = Loc;
  AC6_RndCamAt = At;
  AC6_RndCamRight = VecSet(AC6_RndMatrView.A[0][0], AC6_RndMatrView.A[1][0], AC6_RndMatrView.A[2][0]);
  AC6_RndCamUp = VecSet(AC6_RndMatrView.A[0][1], AC6_RndMatrView.A[1][1], AC6_RndMatrView.A[2][1]);
  AC6_RndCamDir = VecSet(AC6_RndMatrView.A[0][2], AC6_RndMatrView.A[1][2], AC6_RndMatrView.A[2][2]);
}






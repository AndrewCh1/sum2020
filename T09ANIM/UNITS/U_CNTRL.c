#include <stdio.h>
#include "../units.h"

typedef struct tagac6UNIT_CONTROL
{
  AC6_UNIT_BASE_FIELDS;
  FLT
    Dist,
    Azimuth,
    Elevator;
} ac6UNIT_CONTROL;

static VOID AC6_UnitInit( ac6UNIT_CONTROL *Uni, ac6ANIM *Ani )
{
  HFONT hFnt, hFntOld;

  Uni->Dist = 8;

  /* Font creation for text drawing */
  hFnt = CreateFont(47, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                    RUSSIAN_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
                    ANTIALIASED_QUALITY, FF_SWISS | VARIABLE_PITCH, "Consolas");

  hFntOld = SelectObject(Ani->hDC, hFnt);
  wglUseFontBitmaps(Ani->hDC, 0, 256, 3030);
  SelectObject(Ani->hDC, hFntOld);
  DeleteObject(hFnt);
}


/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       vac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitResponse( ac6UNIT_CONTROL *Uni, ac6ANIM *Ani )
{
  static CHAR Buf[102];

   if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if (Ani->KeysClick['S'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



  Uni->Dist += Ani->GlobalDeltaTime * (-2 * Ani->Mdz + 8 * (1 + Ani->Keys[VK_SHIFT] * 30) * (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]) );
  Uni->Azimuth += Ani->GlobalDeltaTime * (-300 * Ani->Keys[VK_LBUTTON] * Ani->Mdx + 150 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));
  Uni->Elevator += Ani->GlobalDeltaTime * (-300 * Ani->Keys[VK_LBUTTON] * Ani->Mdy + 47 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));

  if (Uni->Elevator > 89.99)
    Uni->Elevator = 89.99;
  else if (Uni->Elevator < -89.99)
    Uni->Elevator = -89.99;
 
  if (Uni->Dist < 1.5)
    Uni->Dist = 1.5;

  AC6_RndCamSet(PointTransform(VecSet(0, 0, Uni->Dist),
                  MatrMulMatr(MatrRotateX(Uni->Elevator),
                              MatrRotateY(Uni->Azimuth))),
                VecSet(0, 0, 0),
                VecSet(0, 1, 0));
} 


/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitRender( ac6UNIT_CONTROL *Uni, ac6ANIM *Ani )
{
  INT n[10], i;
  MATR m = MatrOrtho(0, Ani->W - 1, Ani->H - 1, 0, -1, 1);
  static CHAR Buf[10][100];
  //  INT n[10], i;
  //  MATR m = MatrOrtho(0, Ani->W - k, Ani->H - k, 0, -1, 1);
  //  static CHAR Buf[10][100];


  n[0] = sprintf(Buf[0], "FPS: %.3f", Ani->FPS);
  n[1] = sprintf(Buf[1], "Renderer: %s", glGetString(GL_RENDERER));
  n[2] = sprintf(Buf[2], "Vendor: %s", glGetString(GL_VENDOR));
  n[3] = sprintf(Buf[3], "Version: %s", glGetString(GL_VERSION));
  n[4] = sprintf(Buf[4], "GLSL ver: %s",
                 glGetString(GL_SHADING_LANGUAGE_VERSION));

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glUseProgram(0);
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glLoadMatrixf(m.A[0]);

  glListBase(3030); /* 3030 - base list index from UnitInit:
                     *       wglUseFontBitmaps(Ani->hDC, 0, 256, 3030); */
  for (i = 0; i < 5; i++)
  {
    glRasterPos2d(10, 47 * (i + 1));
    glColor3d(0, 0, 0);
    glCallLists(n[i], GL_UNSIGNED_BYTE, Buf[i]);

    glRasterPos3d(10 + 3, 47 * (i + 1) + 3, -0.1);
    glColor3d(1, 1, 1);
    glCallLists(n[i], GL_UNSIGNED_BYTE, Buf[i]);
  }
  glPopAttrib();
}
/* End of 'VG4_UnitRender' function */


ac6UNIT * AC6_UnitCreateControl( VOID )
{
  ac6UNIT *Uni;

  if ((Uni = AC6_AnimUnitCreate(sizeof(ac6UNIT_CONTROL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AC6_UnitInit;
  Uni->Response = (VOID *)AC6_UnitResponse;
  Uni->Render = (VOID *)AC6_UnitRender;

  return Uni;
} /* End of 'VG4_UnitCreateBall' function */
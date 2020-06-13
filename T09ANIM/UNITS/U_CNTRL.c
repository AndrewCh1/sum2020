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
   if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else if (Ani->KeysClick['S'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



  Uni->Dist += Ani->GlobalDeltaTime * (2 * Ani->Mdz + 8 * (1 + Ani->Keys[VK_SHIFT] * 30) * (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]) );
  Uni->Azimuth += Ani->GlobalDeltaTime * (-300 * Ani->Keys[VK_LBUTTON] * Ani->Mdx + 150 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));
  Uni->Elevator += Ani->GlobalDeltaTime * (-300 * Ani->Keys[VK_LBUTTON] * Ani->Mdy + 47 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));

  if (Uni->Elevator > 89.99)
    Uni->Elevator = 89.99;
  else if (Uni->Elevator < -89.99)
    Uni->Elevator = -89.99;

  if (Uni->Dist < 10)
    Uni->Dist = 10;

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
  INT n;
  MATR m = MatrOrtho(0, Ani->W - 1, Ani->H - 1, 0, -1, 1);
  static CHAR Buf[100];

  n = sprintf(Buf, "FPS: %.3f", Ani->FPS);

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glLoadMatrixf(m.A[0]);
  glRasterPos2d(10, 30);

  glListBase(3030);
  glCallLists(n, GL_UNSIGNED_BYTE, Buf);
  glEnable(GL_DEPTH_TEST);
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
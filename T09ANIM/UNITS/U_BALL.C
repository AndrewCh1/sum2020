
#include "../units.h"

typedef struct tagac6UNIT_BALL
{
  AC6_UNIT_BASE_FIELDS;
  ac6PRIM Ball;
} ac6UNIT_BALL;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitInit( ac6UNIT_BALL *Uni, ac6ANIM *Ani )
{
  AC6_RndPrimCreateSphere(&Uni->Ball, VecSet(0, 0, 0), 1, 80, 10);
} 
/* End of 'AC6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitClose( ac6UNIT_BALL *Uni, ac6ANIM *Ani )
{
  AC6_RndPrimFree(&Uni->Ball);
} 
/* End of 'AC6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       vac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitResponse( ac6UNIT_BALL *Uni, ac6ANIM *Ani )
{
} 
/* End of 'AC6_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitRender( ac6UNIT_BALL *Uni, ac6ANIM *Ani )
{
  AC6_RndPrimDraw(&Uni->Ball, MatrRotate(Ani->Time * 30, VecSet(1, 2, 3)));
}
/* End of 'VG4_UnitRender' function */

ac6UNIT * AC6_UnitCreateBall( VOID )
{
  ac6UNIT *Uni;

  if ((Uni = AC6_AnimUnitCreate(sizeof(ac6UNIT_BALL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AC6_UnitInit;
  Uni->Close = (VOID *)AC6_UnitClose;
  Uni->Response = (VOID *)AC6_UnitResponse;
  Uni->Render = (VOID *)AC6_UnitRender;

  return Uni;
} /* End of 'VG4_UnitCreateBall' function */

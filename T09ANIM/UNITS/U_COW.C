#include "../units.h"

typedef struct tagac6UNIT_COW
{
  AC6_UNIT_BASE_FIELDS;
  ac6PRIM Cow;
} ac6UNIT_COW;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitInit( ac6UNIT_COW *Uni, ac6ANIM *Ani )
{
  AC6_RndPrimLoad( &Uni->Cow, "cow.obj" );
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
static VOID AC6_UnitClose( ac6UNIT_COW *Uni, ac6ANIM *Ani )
{
  AC6_RndPrimFree(&Uni->Cow);
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
static VOID AC6_UnitResponse( ac6UNIT_COW *Uni, ac6ANIM *Ani )
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
static VOID AC6_UnitRender( ac6UNIT_COW *Uni, ac6ANIM *Ani )
{
  AC6_RndPrimDraw(&Uni->Cow, MatrRotate(Ani->Time * 30, VecSet(1, 2, 3)));
}
/* End of 'VG4_UnitRender' function */

ac6UNIT * AC6_UnitCreateCow( VOID )
{
  ac6UNIT *Uni;

  if ((Uni = AC6_AnimUnitCreate(sizeof(ac6UNIT_COW))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AC6_UnitInit;
  Uni->Close = (VOID *)AC6_UnitClose;
  Uni->Response = (VOID *)AC6_UnitResponse;
  Uni->Render = (VOID *)AC6_UnitRender;

  return Uni;
} /* End of 'VG4_UnitCreateBall' function */
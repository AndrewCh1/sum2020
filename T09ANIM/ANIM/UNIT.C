
#include "anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitInit( ac6UNIT *Uni, ac6ANIM *Ani )
{
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
static VOID AC6_UnitClose( ac6UNIT *Uni, ac6ANIM *Ani )
{
} 
/* End of 'AC6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitResponse( ac6UNIT *Uni, ac6ANIM *Ani )
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
static VOID AC6_UnitRender( ac6UNIT *Uni, ac6ANIM *Ani )
{
}
/* End of 'AC6_UnitRender' function */

ac6UNIT * AC6_AnimUnitCreate( INT Size )
{
  ac6UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(ac6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);

  /* Setup unit methods */
  Uni->Init = AC6_UnitInit;
  Uni->Close = AC6_UnitClose;
  Uni->Response = AC6_UnitResponse;
  Uni->Render = AC6_UnitRender;

  return Uni;
} /* End of 'VG4_AnimUnitCreate' function */




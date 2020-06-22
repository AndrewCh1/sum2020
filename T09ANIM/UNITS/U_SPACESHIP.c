#include "../units.h"

typedef struct tagac6UNIT_SPACESHIP
{
  AC6_UNIT_BASE_FIELDS;
  INT TexId;
  ac6PRIMS Model;
  VEC Pos;
  FLT Head;
  FLT Speed;
} ac6UNIT_SPACESHIP;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitInit( ac6UNIT_SPACESHIP *Uni, ac6ANIM *Ani )
{
  ac6MATERIAL mtl = AC6_RndMtlGetDef();
  
  mtl.Tex[0] = 0;
  mtl.Kd = VecSet1(0.2245);
  mtl.Ks = VecSet1(0.0645);
  mtl.Ka = VecSet1(0.2245);
  mtl.Ph = 83.2;
  mtl.ShdNo = 0;
  Uni->Model.MtlNo = AC6_RndMtlAdd(&mtl);
  AC6_RndPrimsLoad(&Uni->Model, "BIN/Models/SShip.g3dm");
  Uni->Model.Trans = MatrMulMatr3(MatrScale(VecSet1(0.0009)), MatrTranslate(VecSet(0, 0, 0)), MatrRotateY(180));
} /* End of 'AC6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitClose( ac6UNIT_SPACESHIP *Uni, ac6ANIM *Ani )
{
  AC6_RndPrimsFree(&Uni->Model);
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
static VOID AC6_UnitResponse( ac6UNIT_SPACESHIP *Uni, ac6ANIM *Ani )
{
  /*Movement*/
  Uni->Head += (Ani->Keys['A'] - Ani->Keys['D']) * 90 * Ani->DeltaTime;
  Uni->Speed += (Ani->Keys['W'] - Ani->Keys['S']) * 0.02 * Ani->DeltaTime;
  Uni->Pos.Y += (Ani->Keys[VK_SPACE] - Ani->Keys[VK_LCONTROL]) * 30 * Ani->DeltaTime;
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(VectorTransform(VecSet(0, 0, -1), MatrRotateY(Uni->Head)), Uni->Speed));
  /*Camera movement*/
  AC6_RndCamSet(VecAddVec(Uni->Pos, VectorTransform(VecSet(0, 2, 7), MatrRotateY(Uni->Head))),
                Uni->Pos, VecSet(0, 1, 0));
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
static VOID AC6_UnitRender( ac6UNIT_SPACESHIP *Uni, ac6ANIM *Ani )
{
  AC6_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrRotateY(Uni->Head),MatrTranslate(Uni->Pos)));
}
/* End of 'VG4_UnitRender' function */

ac6UNIT * AC6_UnitCreateSpaceship( VOID )
{
  ac6UNIT *Uni;

  if ((Uni = AC6_AnimUnitCreate(sizeof(ac6UNIT_SPACESHIP))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AC6_UnitInit;
  Uni->Close = (VOID *)AC6_UnitClose;
  Uni->Response = (VOID *)AC6_UnitResponse;
  Uni->Render = (VOID *)AC6_UnitRender;

  return Uni;
} /* End of 'AC6_UnitCreateCow' function */

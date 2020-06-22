#include <string.h>

#include "../units.h"

#define SIZE 256

typedef struct tagac6UNIT_SHADOW
{
  AC6_UNIT_BASE_FIELDS;
  ac6PRIM Pr;
  INT FBO;
} ac6UNIT_SHADOW;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
/*AC6_RndShdAdd("SHADOW")*/

static VOID AC6_UnitInit( ac6UNIT_SHADOW *Uni, ac6ANIM *Ani )
{
  INT w, h, fmt, status;
  ac6MATERIAL mtl = AC6_RndMtlGetDef();

  strncpy(mtl.Name, "Shadow", AC6_STR_MAX - 1);
  mtl.ShdNo = AC6_RndShdAdd("SHADOW");
  mtl.Tex[0] = AC6_RndTexAddFmt( "ShadowMap", SIZE, SIZE, GL_DEPTH_COMPONENT24 );

  AC6_RndPrimCreate(&Uni->Pr, NULL, NULL, 1, 0, AC6_RND_PRIM_POINTS);
  Uni->Pr.MtlNo = AC6_RndMtlAdd(&mtl);

  /* FBO */
  glGenFramebuffers(1, &Uni->FBO); 
  glBindFramebuffer(GL_FRAMEBUFFER, Uni->FBO);

  /* Attach create depth texture */
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
    GL_TEXTURE_2D, AC6_RndTextures[mtl.Tex[0]].TexId, 0); 

  glBindTexture(GL_TEXTURE_2D, AC6_RndTextures[mtl.Tex[0]].TexId);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &fmt);

  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
    OutputDebugStr("Error in framebuffer");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

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
static VOID AC6_UnitClose( ac6UNIT_SHADOW *Uni, ac6ANIM *Ani )
{
  glBindFramebuffer(GL_FRAMEBUFFER, Uni->FBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0); 
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &Uni->FBO);

  AC6_RndPrimFree(&Uni->Pr);
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
static VOID AC6_UnitResponse( ac6UNIT_SHADOW *Uni, ac6ANIM *Ani )
{

  FLT clear_depth = 1;
  VEC a = AC6_RndCamLoc, b = AC6_RndCamAt;

  AC6_RndCamSet(VecSet(0, 8, 3), VecSet(0, 0, 0), VecSet(0, 1, 1));
  glBindFramebuffer(GL_FRAMEBUFFER, Uni->FBO);
  glViewport(0, 0, SIZE, SIZE);
  glClearBufferfv(GL_DEPTH, 0, &clear_depth);
  Ani->Units[0]->Render(Ani->Units[0], Ani);
  glFinish();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, Ani->W, Ani->H);

  AC6_RndCamSet(a, b, VecSet(0, 1, 0));
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
static VOID AC6_UnitRender( ac6UNIT_SHADOW *Uni, ac6ANIM *Ani )
{
  AC6_RndPrimDraw(&Uni->Pr, MatrIdentity());
}
/* End of 'VG4_UnitRender' function */

ac6UNIT * AC6_UnitCreateShadow( VOID )
{
  ac6UNIT *Uni;

  if ((Uni = AC6_AnimUnitCreate(sizeof(ac6UNIT_SHADOW))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AC6_UnitInit;
  Uni->Close = (VOID *)AC6_UnitClose;
  Uni->Response = (VOID *)AC6_UnitResponse;
  Uni->Render = (VOID *)AC6_UnitRender;

  return Uni;
} /* End of 'AC6_UnitCreateCow' function */

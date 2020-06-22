/* FILE NAME: RNDLIGHT.C
 * PROGRAMMER: AC6
 * DATE: 18.06.2020
 * PURPOSE: 3D animation project.
 *          Light and shadow handle module.
 */

#include "rnd.h"

/* Shadow map size */ 
#define AC6_RND_SHADOW_MAP_SIZE 16384

/* Shadow frame buffer identifier */ 
static INT AC6_RndShadowFBO;

/* Shadow map texture identifier */ 
INT AC6_RndShadowTexId;

/* Shadow map light source matrix */ 
MATR AC6_RndShadowMatr;

/* Flag for shadow drawing rendering pass */ 
BOOL AC6_RndShadowPassFlag;

/* Light source direction */ 
VEC AC6_RndLightDir;

/* Light source color */ 
VEC AC6_RndLightColor;

/* Light system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AC6_RndLightInit( VOID )
{
  INT tex_no, status;

  /* Set default light source */
  AC6_RndLightDir = VecNormalize(VecSet(12, 8, 3));
  AC6_RndLightColor = VecSet(1, 1, 1);

  /* Create shadow map texture */
  tex_no = AC6_RndTexAddFmt("ShadowMap", AC6_RND_SHADOW_MAP_SIZE, AC6_RND_SHADOW_MAP_SIZE, GL_DEPTH_COMPONENT32);
  AC6_RndShadowTexId = AC6_RndTextures[tex_no].TexId;

  /* Create shadow FBO */
  glGenFramebuffers(1, &AC6_RndShadowFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, AC6_RndShadowFBO);

  /* Attach create depth texture */
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, AC6_RndShadowTexId, 0);
  /*
  glBindTexture(GL_TEXTURE_2D, AC6_RndShadowTexId);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &fmt);
  */

  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
    OutputDebugStr("Error in framebuffer\n");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
} /* End of 'AC6_RndLightInit' function */

/* Light system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AC6_RndLightClose( VOID )
{
  /* Remove FBO */
  glBindFramebuffer(GL_FRAMEBUFFER, AC6_RndShadowFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &AC6_RndShadowFBO);
  AC6_RndShadowFBO = 0;
} /* End of 'AC6_RndLightClose' function */

/* Draw shadow map function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AC6_RndLightShadow( VOID )
{
  INT i;
  FLT clear_depth = 1, size = 300, far_dist = 300;
  MATR m1, m2, m3;
  VEC
    RndCamLoc = AC6_RndCamLoc,
    RndCamAt = AC6_RndCamAt,
    RndCamRight = AC6_RndCamRight,
    RndCamUp = AC6_RndCamUp,
    RndCamDir = AC6_RndCamDir;


  /* Save camera data */
  m1 = AC6_RndMatrView;
  m2 = AC6_RndMatrProj;
  m3 = AC6_RndMatrVP;

  /* Set camera from light source */
  AC6_RndCamSet(VecSet(0, 8, 3), VecSet(0, 0, 0), VecSet(0, 1, 0));

  AC6_RndMatrProj = MatrOrtho(-size, size, -size, size, -size, far_dist);
  /* AC6_RndMatrView = MatrView(AC6_RndCamAt, VecSubVec(AC6_RndCamAt, AC6_RndLightDir), VecSet(0, 1, 0)); */
  /* AC6_RndMatrVP = MatrMulMatr(AC6_RndMatrView, AC6_RndMatrProj); */
  AC6_RndCamSet(RndCamAt, VecSubVec(RndCamAt, AC6_RndLightDir), VecSet(0, 1, 0));
  AC6_RndShadowMatr = AC6_RndMatrVP;

  glViewport(0, 0, AC6_RND_SHADOW_MAP_SIZE, AC6_RND_SHADOW_MAP_SIZE);

  /* Set shadow map frame buffer */
  glBindFramebuffer(GL_FRAMEBUFFER, AC6_RndShadowFBO);

  /* Clear depth */
  glClearBufferfv(GL_DEPTH, 0, &clear_depth);

  /* Redraw all scene */
  AC6_RndShadowPassFlag = TRUE;
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(4, 4);
  for (i = 0; i < AC6_Anim.NumOfUnits; i++)
    AC6_Anim.Units[i]->Render(AC6_Anim.Units[i], &AC6_Anim);
  AC6_RndShadowPassFlag = FALSE;
  glDisable(GL_POLYGON_OFFSET_FILL);

  /* Restore main frame buffer */
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, AC6_RndFrameW, AC6_RndFrameH);

  /* Restore camera data */
  AC6_RndMatrView = m1;
  AC6_RndMatrProj = m2;
  AC6_RndMatrVP = m3;
  AC6_RndCamLoc = RndCamLoc;
  AC6_RndCamAt = RndCamAt;
  AC6_RndCamRight = RndCamRight;
  AC6_RndCamUp = RndCamUp;
  AC6_RndCamDir = RndCamDir;
} /* End of 'AC6_RndLightShadow' function */

/* END OF 'RNDLIGHT.C' FILE */

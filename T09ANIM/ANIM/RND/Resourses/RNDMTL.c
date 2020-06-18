#include <stdio.h>
#include <string.h>

#include "../rnd.h"


ac6MATERIAL AC6_RndMaterials[AC6_MAX_MATERIALS]; /* Array of materials */
INT AC6_RndMaterialsSize;                        /* Materials array store size */

VOID AC6_RndMtlInit( VOID )
{
  INT i;
  ac6MATERIAL mtl;
  AC6_RndMaterialsSize = 0;

  strncpy(mtl.Name, "Default", AC6_STR_MAX - 1);

  memset(&mtl, 0, sizeof(mtl));
  mtl.Ka = VecSet1(0.1);
  mtl.Kd = VecSet1(0.9);
  mtl.Ks = VecSet1(0.0);
  mtl.Ph = 30;
  mtl.Trans = 1;
  for(i = 0; i < 8; i++)
    mtl.Tex[i] = -1;
  mtl.ShdNo = 0;
  AC6_RndMtlAdd(&mtl);

}

VOID AC6_RndMtlClose( VOID )
{
  AC6_RndMaterialsSize = 0;
}

INT AC6_RndMtlAdd( ac6MATERIAL *Mtl )
{
  if (AC6_RndMaterialsSize >= AC6_MAX_MATERIALS)
    return 0;

  AC6_RndMaterials[AC6_RndMaterialsSize] = *Mtl;
  return AC6_RndMaterialsSize++;
}

ac6MATERIAL AC6_RndMtlGetDef( VOID )
{
  return AC6_RndMaterials[0];
}

ac6MATERIAL * AC6_RndMtlGet( INT MtlNo )
{
  /* Get material pointer */
  if (MtlNo < 0 || MtlNo >= AC6_RndMaterialsSize)
    MtlNo = 0;
  return &AC6_RndMaterials[MtlNo];
}

/*Material apply function*/
INT AC6_RndMtlApply( INT MtlNo )
{
  INT prg, loc, i;
  ac6MATERIAL *mtl;
 
  /* Set material pointer 
  if (MtlNo < 0 || MtlNo >= AC6_RndMaterialsSize)
    MtlNo = 0;
  mtl = &AC6_RndMaterials[MtlNo];
  */
  mtl = AC6_RndMtlGet(MtlNo);
  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= AC6_RndShadersSize)
    prg = 0;
  prg = AC6_RndShaders[prg].ProgId;

  glUseProgram(prg);
  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);

  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsMaterial0";

    tname[9] = '0' + i;
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, AC6_RndTextures[mtl->Tex[i]].TexId);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }
  return prg;
} /* End of 'AC6_RndMtlApply' function */




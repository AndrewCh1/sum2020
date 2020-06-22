/* FILE NAME: RNDTEX.C
 * PROGRAMMER: AC6
 * DATE: 11.06.2020
 * PURPOSE: 3D animation project.
 *          Render system shader handle module.
 */

#include <stdio.h>
#include <string.h>




#include "../rnd.h"


ac6TEXTURE AC6_RndTextures[AC6_MAX_TEXTURES]; /* Array of textures */
INT AC6_RndTexturesSize;



/* Load text from file function.
 * ARGUMENTS:
 *   - file name to be load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text (allocated through malloc).
 */

/* Texture init function.
 * ARGUMENTS:None. 
 * RETURNS: None.
 */
VOID AC6_RndTexInit( VOID )
{
  AC6_RndTexturesSize = 0;
} /* End of 'AC6_RndTexInit' function */

/* Closing shader function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AC6_RndTexClose( VOID )
{
  INT i;

  for(i = 0; i < AC6_RndTexturesSize; i++)
    glDeleteTextures(1, &AC6_RndTextures[i].TexId);
  AC6_RndTexturesSize = 0;
}  /* End of 'AC6_RndTexClose' function */


INT AC6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{

  if (AC6_RndTexturesSize >= AC6_MAX_TEXTURES)
    return -1;

  strncpy(AC6_RndTextures[AC6_RndTexturesSize].Name, Name, AC6_STR_MAX - 1);
  AC6_RndTextures[AC6_RndTexturesSize].W = W;
  AC6_RndTextures[AC6_RndTexturesSize].H = H;
  glGenTextures(1, &AC6_RndTextures[AC6_RndTexturesSize].TexId);

  glBindTexture(GL_TEXTURE_2D, AC6_RndTextures[AC6_RndTexturesSize].TexId);
  glTexImage2D(GL_TEXTURE_2D, 0, C, H, W, 0, C == 3 ? GL_BGR : C == 4 ? GL_BGRA : GL_LUMINANCE, GL_UNSIGNED_BYTE, Bits);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  return AC6_RndTexturesSize++;
}


INT AC6_RndTexAddFmt( CHAR *Name, INT W, INT H, INT GLType )
{
  /* Check for free space */
  if (AC6_RndTexturesSize >= AC6_MAX_TEXTURES)
    return -1;

  strncpy(AC6_RndTextures[AC6_RndTexturesSize].Name, Name, AC6_STR_MAX - 1);
  AC6_RndTextures[AC6_RndTexturesSize].W = W;
  AC6_RndTextures[AC6_RndTexturesSize].H = H;
  glGenTextures(1, &AC6_RndTextures[AC6_RndTexturesSize].TexId);

  glBindTexture(GL_TEXTURE_2D, AC6_RndTextures[AC6_RndTexturesSize].TexId);

  glTexStorage2D(GL_TEXTURE_2D, 1, GLType, W, H);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  return AC6_RndTexturesSize++;
} /* End of 'AC6_RndTexAddFmt' function */

 
/* Adding texture function.
 * ARGUMENTS:
 *   - message file prefix, shader name and text:
 *       CHAR *Prefix, *PartName, *Text;
 * RETURNS: None.
 */
INT AC6_RndTexAdd( CHAR *FileName )
{
  INT n;
  ac6IMAGE Img;

  if (!AC6_ImgLoad(&Img, FileName))
    return -1;
  n = AC6_RndTexAddImg(FileName, Img.W, Img.H, 4, Img.Pixels);
  AC6_ImgFree(&Img);
  return n;
}/* End of 'AC6_RndTexAdd' func */

/* END OF 'RNDTEX.C' FILE */
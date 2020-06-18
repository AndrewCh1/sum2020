/* FILE NAME: RNDTEX.C
 * PROGRAMMER: AC6
 * DATE: 11.06.2020
 * PURPOSE: 3D animation project.
 *          Render system shader handle module.
 */

#include <stdio.h>
#include <string.h>




#include "../rnd.h"

#define AC6_STR_MAX 300

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
    return 0;

  strncpy(AC6_RndTextures[AC6_RndTexturesSize].Name, Name, AC6_STR_MAX - 1);
  AC6_RndTextures[AC6_RndTexturesSize].W = W;
  AC6_RndTextures[AC6_RndTexturesSize].H = H;
  glGenTextures(1, &AC6_RndTextures[AC6_RndTexturesSize].TexId);

  glBindTexture(GL_TEXTURE_2D, AC6_RndTextures[AC6_RndTexturesSize].TexId);
  glTexImage2D(GL_TEXTURE_2D, 0, C, H, W, 0, C == 3 ? GL_BGR : C == 4 ? GL_BGRA : GL_LUMINANCE, GL_UNSIGNED_BYTE, Bits);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  return AC6_RndTexturesSize++;
}
 
/* Adding texture function.
 * ARGUMENTS:
 *   - message file prefix, shader name and text:
 *       CHAR *Prefix, *PartName, *Text;
 * RETURNS: None.
 */
INT AC6_RndTexAdd( CHAR *FileName )
{
INT k;
  HBITMAP hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
  BITMAP bm;

  if (hBm == NULL)
  return -1;
  GetObject(hBm, sizeof(bm), &bm);
  k = AC6_RndTexAddImg(FileName, bm.bmWidth, bm.bmHeight, bm.bmBitsPixel / 8, bm.bmBits);
  DeleteObject(hBm);
  return k;
}/* End of 'AC6_RndTexAdd' fn */

/* END OF 'RNDTEX.C' FILE */
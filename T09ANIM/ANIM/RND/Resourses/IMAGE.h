/* FILE NAME: IMAGE.H
 * PROGRAMMER: AC6
 * DATE: 13.06.2020
 * PURPOSE: 3D animation project.
 *          Image constract module.
 */

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "rndres.h"

typedef struct tagac6IMAGE
{
  INT W, H;           /*Image size */
  BYTE *Pixels;       /* Pixel array */
  DWORD *PixelsDW;    /* Pixel array */
  BYTE (*PixelsC)[4]; /* Pixel array */
} ac6IMAGE;

/* Create image function.
 * ARGUMENTS:
 *   - pointer to image to create:
 *       ac6IMAGE *Im;
 *   - size of image:
 *       INT W, H;
 * RETURNS:
 *  (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AC6_ImgCreate( ac6IMAGE *Im, INT W, INT H );

/* Load image function.
 * ARGUMENTS:
 *   - pointer to image to create:
 *       ac6IMAGE *Im;
 *   - name of image:
 *       CHAR *FileName;
 * RETURNS:
 *  (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AC6_ImgLoad( ac6IMAGE *Im, CHAR *FileName );

/* Create image function.
 * ARGUMENTS:
 *   - pointer to image to create:
 *       ac6IMAGE *Im;
 * RETURNS: None.
 */
VOID AC6_ImgFree( ac6IMAGE *Im );

#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
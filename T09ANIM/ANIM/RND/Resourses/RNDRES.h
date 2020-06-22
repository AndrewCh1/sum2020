#ifndef __RNDRES_H_
#define __RNDRES_H_
#include "../../../def.h"

#define AC6_STR_MAX  300
#define AC6_MAX_TEXTURES 30
#define AC6_MAX_SHADERS  47
#define AC6_MAX_MATERIALS 50

typedef struct tagac6SHADER
{
  CHAR Name[AC6_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT ProgId;             /* OpenGL texture Id */ 
} ac6SHADER;

extern ac6SHADER AC6_RndShaders[AC6_MAX_SHADERS];
extern INT AC6_RndShadersSize;

VOID AC6_RndShdInit( VOID ); 
VOID AC6_RndShdClose( VOID ); 
VOID AC6_RndShdUpdate( VOID );
INT AC6_RndShdAdd( CHAR *FileNamePrefix );

typedef struct tagac6TEXTURE
{
  CHAR Name[AC6_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} ac6TEXTURE;

extern ac6TEXTURE AC6_RndTextures[AC6_MAX_TEXTURES]; /* Array of textures */
extern INT AC6_RndTexturesSize;

VOID AC6_RndTexInit( VOID );
VOID AC6_RndTexClose( VOID );
INT AC6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );
INT AC6_RndTexAdd( CHAR *FileName );
INT AC6_RndTexAddFmt( CHAR *Name, INT W, INT H, INT GLType );

typedef struct tagac6MATERIAL
{
  CHAR Name[AC6_STR_MAX]; /* Material name */

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */
  FLT Trans;                /* Transparency factor */
  INT Tex[8];               /* Texture references from texture table (or -1) */

  INT ShdNo;                /* Shader number in shader array */
} ac6MATERIAL;

extern ac6MATERIAL AC6_RndMaterials[AC6_MAX_MATERIALS]; /* Array of materials */
extern INT AC6_RndMaterialsSize;


VOID AC6_RndMtlInit( VOID );
VOID AC6_RndMtlClose( VOID );
INT AC6_RndMtlAdd( ac6MATERIAL *Mtl );
INT AC6_RndMtlApply( INT MtlNo );
ac6MATERIAL AC6_RndMtlGetDef( VOID );
ac6MATERIAL * AC6_RndMtlGet( INT MtlNo );

VOID AC6_RndResInit( VOID );
VOID AC6_RndResClose( VOID );
#endif
#include <string.h>
#include <stdio.h>

#include "rnd.h"

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ac6PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL AC6_RndPrimsCreate( ac6PRIMS *Prs, INT NumOfPrims )
{
  memset(Prs, 0, sizeof(ac6PRIMS));

  if ((Prs->Prims = malloc(sizeof(ac6PRIM) * NumOfPrims)) == NULL)
    return FALSE;
  memset(Prs->Prims, 0, sizeof(ac6PRIM) * NumOfPrims);
  Prs->NumOfPrims = NumOfPrims;
  Prs->Trans = MatrIdentity();
  return TRUE;
} /* End of 'AC6_RndPrimsCreate' function */

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ac6PRIMS *Prs;
 * RETURNS: None.
 */
VOID AC6_RndPrimsFree( ac6PRIMS *Prs )
{
  if (Prs->Prims != NULL)
    free(Prs->Prims);
  memset(Prs, 0, sizeof(ac6PRIMS));
} /* End of 'AC6_RndPrimsFree' function */

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ac6PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID AC6_RndPrimsDraw( ac6PRIMS *Prs, MATR World )
{
  INT i;
  World = MatrMulMatr(Prs->Trans, World);
   for (i = 0; i < Prs->NumOfPrims; i++)
    if (AC6_RndMtlGet(Prs->Prims[i].MtlNo)->Trans == 1)
      AC6_RndPrimDraw(&Prs->Prims[i], World);

  /* Draw all transparent primitives */
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (AC6_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      AC6_RndPrimDraw(&Prs->Prims[i], World);
  glCullFace(GL_BACK);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (AC6_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      AC6_RndPrimDraw(&Prs->Prims[i], World);
  glDisable(GL_CULL_FACE);
} /* End of 'AC6_RndPrimsDraw' function */

BOOL AC6_RndPrimsLoad( ac6PRIMS *Prs, CHAR *FileName )
{
  BYTE *mem, *ptr;
  INT flen, p, t, m;
  FILE *F;
  DWORD Sign; /* == "G3DM" */
  INT NumOfPrims;
  INT NumOfMaterials;
  INT NumOfTextures;
  struct tagG3DMMTL
  {
    CHAR Name[300]; /* Material name */
    /* Illumination coefficients */
    VEC Ka, Kd, Ks;     /* Ambient, diffuse, specular coefficients */
    FLT Ph;             /* Phong power coefficient – shininess */
    FLT Trans;          /* Transparency factor */
    DWORD Tex[8];       /* Texture references 
                        * (8 time: texture number in G3DM file, -1 if no texture) */
    /* Shader information */
    CHAR ShaderString[300]; /* Additional shader information */
    DWORD Shader;       /* Shader number (uses after load into memory) */
  } *mtls;


  memset(Prs, 0, sizeof(ac6PRIMS));

  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* Obtain file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  rewind(F);

  /* Allocate memory and load whole file to memory */
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  fread(mem, 1, flen, F);
  fclose(F);

  ptr = mem;

  Sign = *(DWORD *)ptr, ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    return FALSE;
  }
  NumOfPrims = *(DWORD *)ptr, ptr += 4;
  NumOfMaterials = *(DWORD *)ptr, ptr += 4;
  NumOfTextures = *(DWORD *)ptr, ptr += 4;

  if (!AC6_RndPrimsCreate(Prs, NumOfPrims))
  {
    free(mem);
    return FALSE;
  }

  /* Primitives */
  for (p = 0; p < NumOfPrims; p++)
  {
    DWORD NumOfVertexes;
    DWORD NumOfFacetIndexes;  /* num of facets * 3 */
    DWORD MtlNo;              /* Material number in table below (in material section) */
    ac6VERTEX *V;
    INT *Ind;

    NumOfVertexes = *(DWORD *)ptr, ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr, ptr += 4;
    MtlNo = *(DWORD *)ptr, ptr += 4;
    V = (ac6VERTEX *)ptr, ptr += sizeof(ac6VERTEX) * NumOfVertexes;
    Ind = (INT *)ptr, ptr += sizeof(INT) * NumOfFacetIndexes;
    AC6_RndPrimCreate(&Prs->Prims[p],
       V, Ind, NumOfVertexes, NumOfFacetIndexes, AC6_RND_PRIM_TRIMESH);
    Prs->Prims[p].MtlNo = MtlNo + AC6_RndMaterialsSize;
  }

  /* Materials */
  mtls = (struct tagG3DMMTL *)ptr, ptr += sizeof(struct tagG3DMMTL) * NumOfMaterials;
  for (m = 0; m < NumOfMaterials; m++)
  {
    INT i;
    ac6MATERIAL mtl = AC6_RndMtlGetDef();

    strncpy(mtl.Name, mtls[m].Name, AC6_STR_MAX - 1);
    mtl.Ka = mtls[m].Ka;
    mtl.Kd = mtls[m].Kd;
    mtl.Ks = mtls[m].Ks;
    mtl.Ph = mtls[m].Ph;
    mtl.Trans = mtls[m].Trans;
    for (i = 0; i < 8; i++)
      mtl.Tex[i] = (mtls[m].Tex[i] == -1 ? -1 : mtls[m].Tex[i] + AC6_RndTexturesSize);
    AC6_RndMtlAdd(&mtl);
  }
  /* Textures */
  for (t = 0; t < NumOfTextures; t++)
  {
    struct
    {
      CHAR Name[300]; /* Texture name */
      DWORD W, H;     /* Texture image size in pixels */
      DWORD C;        /* Texture image components (1-mono, 3-bgr or 4-bgra) */
    } *tex = (VOID *)ptr;

    ptr += sizeof(*tex);
    AC6_RndTexAddImg(tex->Name, tex->W, tex->H, tex->C, ptr);
    ptr += tex->W * tex->H * tex->C;
  }

  free(mem);
  return TRUE;
} /* End of 'AC6_RndPrimsLoad' function */




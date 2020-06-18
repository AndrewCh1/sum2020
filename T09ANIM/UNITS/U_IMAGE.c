#include "../units.h"
#include "../anim/rnd/resourses/image.h"

typedef struct tagac6UNIT_IMAGE
{
  AC6_UNIT_BASE_FIELDS;
  ac6PRIM Img;
} ac6UNIT_IMAGE;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitInit( ac6UNIT_IMAGE *Uni, ac6ANIM *Ani )
{
  ac6IMAGE img1, img2;
  ac6VERTEX *V;
  INT x, y, b, g, r, a, d;

  if (AC6_ImgLoad(&img1, "hf.bmp") && AC6_ImgLoad(&img2, "hftex.bmp") && (V = malloc(sizeof(ac6VERTEX) * img1.H * img1.W)) != NULL)
  {
    for (y = 0; y < img1.H; y++)
      for (x = 0; x < img1.W; x++)
      {
        b = img2.Pixels[(img2.W * y + x) * 4 + 0];
        g = img2.Pixels[(img2.W * y + x) * 4 + 1];
        r = img2.Pixels[(img2.W * y + x) * 4 + 2];
        a = img2.Pixels[(img2.W * y + x) * 4 + 3];
        
        d = img1.Pixels[(img1.W * y + x) * 4 + 1];

        V[img1.W * y + x].P = VecSet(x - img1.W / 3, d, img1.H / 3 - y);
        V[img1.W * y + x].C = Vec4Set(r / 255.0, g / 255.0, b / 255.0, 1);
      }
    AC6_RndPrimCreateFromGrid(&Uni->Img, V, img1.W, img1.H, TRUE);
    Uni->Img.MtlNo = 0;
  }

  AC6_ImgFree(&img1);
  AC6_ImgFree(&img2);
  free(V);
} /* End of 'AC6_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       ac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitClose( ac6UNIT_IMAGE *Uni, ac6ANIM *Ani )
{
  AC6_RndPrimFree(&Uni->Img);
} /* End of 'AC6_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ac6UNIT *Uni;
 *   - animation context:
 *       vac6ANIM *Ani;
 * RETURNS: None.
 */
static VOID AC6_UnitResponse( ac6UNIT_IMAGE *Uni, ac6ANIM *Ani )
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
static VOID AC6_UnitRender( ac6UNIT_IMAGE *Uni, ac6ANIM *Ani )
{
  AC6_RndPrimDraw(&Uni->Img, MatrIdentity());

}
/* End of 'VG4_UnitRender' function */

ac6UNIT * AC6_UnitCreateImg( VOID )
{
  ac6UNIT *Uni;

  if ((Uni = AC6_AnimUnitCreate(sizeof(ac6UNIT_IMAGE))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)AC6_UnitInit;
  Uni->Close = (VOID *)AC6_UnitClose;
  Uni->Response = (VOID *)AC6_UnitResponse;
  Uni->Render = (VOID *)AC6_UnitRender;

  return Uni;
} /* End of 'AC6_UnitCreateCow' function */
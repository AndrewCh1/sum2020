/**/

#include <windows.h>
#include "globe.h"
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

static VEC Geom[Globe_H][Globe_W];
static CenterX, CenterY;

VOID GlobeSet( DBL Xc, DBL Yc, DBL R )
{
  INT i, j;
  DBL phi, theta;
  CenterX = (INT)Xc;
  CenterY = (INT)Yc;
  for (i = 0, theta = 0; i < Globe_H; i++, theta += PI / (Globe_H - 1))
    for (j = 0, phi = 0; j < Globe_W; j++, phi += 2 * PI / (Globe_W - 1))
    {
      Geom[i][j].X = R * sin(theta) * sin(phi);
      Geom[i][j].Y = R * cos(theta);
      Geom[i][j].Z = R * sin(theta) * cos(phi);
    }
}

VEC VecRotateZ (VEC V, DBL Angle)
{
  VEC r;

  Angle *= PI / 180;
  r.X = V.X * cos(Angle) - V.Y * sin(Angle);
  r.Y = V.X * sin(Angle) + V.Y * cos(Angle);
  r.Z = V.Z;
  return r;
}

VEC VecRotateX (VEC V, DBL Angle)
{
  VEC r;

  Angle *= PI / 180;
  r.X = V.X; 
  r.Y = V.Z * sin(Angle) + V.Y * cos(Angle);
  r.Z = V.Z * cos(Angle) - V.Y * sin(Angle);
  return r;
}

VOID GlobeDraw( HDC hDC )
{
  INT i, j, s = 2;
  static VEC Geom_1[Globe_H][Globe_W];
  DBL t = (DBL)clock() / CLOCKS_PER_SEC;

  for (i = 0; i < Globe_H; i++)
    for (j = 0; j < Globe_W; j++)
    {
      VEC v, v1;

      v1 = VecRotateX(Geom[i][j], 10 + t * 8);
      v = VecRotateZ(v1, 10);
      Geom_1[i][j] = v;
    }

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));

  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(0, 0, 255));

  for (i = 0; i < Globe_H; i++)
    for (j = 0; j < Globe_W; j++)
    {
      Ellipse(hDC,
        CenterX + Geom_1[i][j].X - s, CenterY + Geom_1[i][j].Y - s,
        CenterX + Geom_1[i][j].X + s, CenterY + Geom_1[i][j].Y + s);
    }
}



#include <windows.h>
#include <math.h>
#include "globe.h"
#define PI 3.14159265358979323846
static VEC Geom[Globe_W][Globe_H];

VOID GlobeSet(INT Xc, INT Yc, INT R)
{
  INT i, j;
  DBL phi, theta;
  for (i = 0, theta = 0; i < Globe_H; i++, theta += PI / (Globe_H - 1))
    for (j = 0, phi = 0; j < Globe_W; j++, phi += 2*PI / (Globe_W - 1))
    {
      Geom[i][j].X = Xc + R * sin(theta) * sin(phi);
      Geom[i][j].Y = Yc + R * cos(theta);
      Geom[i][j].Z = R * sin(theta) * cos(phi);
    }
}

VOID GlobeDraw( HDC hDC )
{
  INT i, j, s = 5;
  for (i = 0; i < Globe_H; i++)
    for (j = 0; j < Globe_W; j++)
    {
      SelectObject(hDC, GetStockObject(DC_BRUSH));
      SelectObject(hDC, GetStockObject(DC_PEN));

      SetDCPenColor(hDC, RGB(0, 0, 0));
      SetDCBrushColor(hDC, RGB(255, 255, 255));
      Ellipse(hDC, Geom[i][j].X - s, Geom[i][j].Y - s, Geom[i][j].X + s, Geom[i][j].Y + s);
    }
}
/*
VOID DrawEye( HDC hDC, INT x, INT y, INT r1, INT r2, INT Mx, INT My )
{
  INT Dx = Mx - x, Dy = My - y;
  INT len = sqrt(Dx * Dx + Dy * Dy);
  INT deltaX = Dx * (r1 - r2) / len, deltaY = Dy * (r1 - r2) / len;
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));

  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Ellipse(hDC, x - r1, y - r1, x + r1, y + r1);
  SetDCPenColor(hDC, RGB(255, 255, 255));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  if (len >= r1 - r2)
    Ellipse(hDC, x + deltaX - r2, y + deltaY - r2, x + deltaX + r2, y + deltaY + r2);
  else
    Ellipse(hDC, Mx - r2, My - r2, Mx + r2, My + r2);
 }  */
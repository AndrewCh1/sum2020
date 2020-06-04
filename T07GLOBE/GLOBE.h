#include <windows.h>

#define Globe_W 5
#define Globe_H 5
typedef DOUBLE DBL;
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

VOID GlobeSet( INT Xc, INT Yc, INT R );
VOID GlobeDraw( HDC hDC );

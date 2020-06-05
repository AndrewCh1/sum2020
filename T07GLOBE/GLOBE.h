#include <windows.h>

#define Globe_W 70
#define Globe_H 70
typedef DOUBLE DBL;
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

VOID GlobeSet( DBL Xc, DBL Yc, DBL R );
VOID GlobeDraw( HDC hDC );

#include "anim.h"

INT AC6_MouseWheel;
static BYTE KeysOld[256];

static VOID AC6_AnimKeyboardInit( VOID )
{
  GetKeyboardState(AC6_Anim.Keys);
}

static VOID AC6_AnimKeyboardResponse( VOID )
{
  INT i;
  GetKeyboardState(AC6_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    AC6_Anim.Keys[i] >>= 7;
    AC6_Anim.KeysClick[i] = AC6_Anim.Keys[i] && !KeysOld[i];
  }

  memcpy(KeysOld, AC6_Anim.Keys, 256);
}

static VOID AC6_AnimMouseInit( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(AC6_Anim.hWnd, &pt);

  /* приращение координат на экране */
  AC6_Anim.Mdx = 0;
  AC6_Anim.Mdy = 00;
  AC6_Anim.Mdz = 0;
  /* абсолютные значения */
  AC6_Anim.Mx = pt.x;
  AC6_Anim.My = pt.y;
  AC6_Anim.Mz = 0;
  AC6_MouseWheel = 0;
}

static VOID AC6_AnimMouseResponse( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(AC6_Anim.hWnd, &pt);

  /* приращение координат на экране */
  AC6_Anim.Mdx = pt.x - AC6_Anim.Mx;
  AC6_Anim.Mdy = pt.y - AC6_Anim.My;
  AC6_Anim.Mdz = AC6_MouseWheel;
  /* абсолютные значения */
  AC6_Anim.Mx = pt.x;
  AC6_Anim.My = pt.y;
  AC6_Anim.Mz += AC6_MouseWheel;
  AC6_MouseWheel = 0;
}

VOID AC6_AnimInputInit( VOID )
{
  AC6_AnimKeyboardInit();
  AC6_AnimMouseInit();
}
VOID AC6_AnimInputResponse( VOID )
{
  AC6_AnimKeyboardResponse();
  AC6_AnimMouseResponse();
}

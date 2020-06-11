#include "anim.h"

ac6ANIM AC6_Anim;
INT AC6_MouseWheel;

VOID AC6_AnimInit( HWND hWnd )
{
  memset(&AC6_Anim, 0, sizeof(ac6ANIM));
  AC6_Anim.hWnd = hWnd;
  AC6_RndInit(hWnd);
  AC6_Anim.hDC = AC6_hRndDC;

  AC6_AnimInputInit();
  AC6_TimerInit();

}

VOID AC6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < AC6_Anim.NumOfUnits; i++)
  {
    AC6_Anim.Units[i]->Close(AC6_Anim.Units[i], &AC6_Anim);
    free(AC6_Anim.Units[i]);
    AC6_Anim.Units[i] = NULL;
  }
  AC6_Anim.NumOfUnits = 0;
  AC6_RndClose();

  memset(&AC6_Anim, 0, sizeof(ac6ANIM));
}

VOID AC6_AnimResize( INT W, INT H )
{
  AC6_Anim.W = W;
  AC6_Anim.H = H;
  AC6_RndResize(W, H);
}

VOID AC6_AnimCopyFrame( VOID )
{
  AC6_RndCopyFrame();
}

VOID AC6_AnimRender( VOID )
{
  INT i;

  AC6_AnimInputResponse();
  AC6_TimerResponse();


  for (i = 0; i < AC6_Anim.NumOfUnits; i++)
    AC6_Anim.Units[i]->Response(AC6_Anim.Units[i], &AC6_Anim);

  AC6_RndStart();
  for (i = 0; i < AC6_Anim.NumOfUnits; i++)
    AC6_Anim.Units[i]->Render(AC6_Anim.Units[i], &AC6_Anim);
  AC6_RndEnd();
}

VOID AC6_AnimAddUnit( ac6UNIT *Uni )
{
  if (AC6_Anim.NumOfUnits < AC6_MAX_UNITS)
  {
    AC6_Anim.Units[AC6_Anim.NumOfUnits] = Uni;
    AC6_Anim.Units[AC6_Anim.NumOfUnits]->Init(AC6_Anim.Units[AC6_Anim.NumOfUnits], &AC6_Anim);
    AC6_Anim.NumOfUnits++;
  }
}

/*VOID AC6_AnimFlipFullScreen( VOID )
{

}
*/

VOID AC6_AnimExit( VOID )
{
  static BOOL IsExit = FALSE;

  if (IsExit)
    return;
  IsExit = TRUE;

  PostMessage(AC6_Anim.hWnd, WM_CLOSE, 0, 0);
}


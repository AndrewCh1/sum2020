/* FILE NAME: MAIN.c
 * PROGRAMMER: AC6
 * DATE: 05.06.2020
 * PURPOSE: 3D animation project.
 *          Main module.
 */

#include "../units.h"
           
#define WND_CLASS_NAME "My Window Class Name"

LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );


/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  HWND hWnd;
  MSG msg;
  WNDCLASS wc;

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = WinFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error regoister window class", "Error", MB_OK | MB_ICONERROR);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "CGSG Primary Practice Group First Window",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    0, 0, 800, 800,
    NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, CmdShow);
  AC6_AnimAddUnit(AC6_UnitCreateBall());
  //AC6_AnimAddUnit(AC6_UnitCreateCow());
  AC6_AnimAddUnit(AC6_UnitCreateControl());

  /* MessageLoop */
  while(TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else 
    {
      AC6_AnimRender();
      AC6_RndCopyFrame();
    }

  return 30;
} /* End of 'WinMain' function */

LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  static ac6PRIM Pr, cow;
  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0; 
  case WM_CREATE:
    AC6_AnimInit(hWnd);
    SetTimer(hWnd, 47, 2, NULL);
    return 0;
  case WM_SIZE:
    AC6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    AC6_AnimRender();
    AC6_RndCopyFrame();
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  case WM_MOUSEWHEEL:
    AC6_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_TIMER:
    AC6_AnimRender();
    AC6_RndCopyFrame();
    /* InvalidateRect(hWnd, NULL, FALSE); */
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    AC6_AnimCopyFrame();
    EndPaint(hWnd, &ps);
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure you want to close window?", "Close",
          MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDYES)
      DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    AC6_AnimClose();
    KillTimer(hWnd, 47);
    PostQuitMessage(30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* End of 'MAIN.c' file */

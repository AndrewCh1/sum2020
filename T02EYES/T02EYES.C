#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define WND_CLASS_NAME "My Window Class Name"

LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID DrawEye( HDC hDC, INT x, INT y, INT r1, INT r2, INT Mx, INT My );

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
    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
     0, 0, 800, 800,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, CmdShow);


  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return 30;
}

LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  POINT pt;
  HPEN hPen, hOldPen;
  static HDC hMemDC;
  static HBITMAP hBm;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
    SetTimer(hWnd, 30, 12, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    DrawEye(hMemDC, 300, 300, 80, 30, pt.x, pt.y);

    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    InvalidateRect(hWnd, NULL, FALSE);
    SelectObject(hMemDC, GetStockObject(WHITE_BRUSH));
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    DrawEye(hMemDC, 300, 300, 80, 30, pt.x, pt.y);
    DrawEye(hMemDC, 600, 600, 80, 30, pt.x, pt.y);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure you want to close window?", "Close",
          MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDYES)
      DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    PostMessage(hWnd, WM_QUIT, 0, 0);
    if (hBm != NULL)
      DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

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
 }  
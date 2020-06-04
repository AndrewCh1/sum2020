#include <stdlib.h>
#include <windows.h>

#define WND_CLASS_NAME "My Window Class Name"

LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

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
  static BITMAP bm;
  static hDC hMemDC, hMemDCFrame, hMemDCClock;
  static HBITMAP hBm, hBmFrame, hBmClock;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    hBmClock = LoadImage(NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmClock, sizeof(bm), &bm);

    SetTimer(hWnd, 30, 102, NULL);
    hDC = GetDC(hWnd);
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDCClock = CreateCompatibleDC(hDC);
    SelectObject(hMemDCClock, hBmClock);
    BitBlt(hMemDCFrame, 0, 0, bm.bmWidth, bm.bmHeight, hMemDCclock, 0, 0, SRCCOPY);

    ReleaseDC(hWnd, hDC);
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    hDC = GetDC(hWnd);
    hBmFrame = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDCFrame, hBmFrame);

    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  case WM_TIMER:
    SelectObject(hMemDCFrame, GetStockObject(WHITE_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    Rectangle(hMemDCF, 0, 0, w + 1, h + 1);

    BitBlt(hMemDCFrame, 0, 0, bm.bmWidth, bm.bmHeight, hMemDCÑlock, 0, 0, SRCCOPY);

    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_LBUTTONDOWN:
  case WM_MOUSEMOVE:
    hDC = GetDC(hWnd);
    if (wParam & MK_LBUTTON)
      Ellipse(hDC, LOWORD(lParam) - 30, HIWORD(lParam) - 30, LOWORD(lParam) + 30, HIWORD(lParam) + 30);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    EndPaint(hWnd, &ps);
    return 0;
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure you want to close window?", "Close",
          MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDYES)
      DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    PostMessage(hWnd, WM_QUIT, 0, 0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);

} /* End of 'WinFunc' function */
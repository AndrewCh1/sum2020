#include <windows.h>

#define WND_CLASS_NAME "My window class"

/* Ссылка на функцию обработки*/
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT msg,
							  WPARAM wParam, LPARAM lParam);

/*Главная функция приложения*/
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				    CHAR *CmdLine, INT ShowCmd )
{


}
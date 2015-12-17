#include <windows.h>
#include <cstdio>
#include <vector>
#include <ctime>
using namespace std;


#define rep(i,n) for(int i=0;i<((int)(n));i++)
#define eprintf(...)  fprintf(stderr,__VA_ARGS__)


#include "rensya_window.h"


HDC CopyWindow(HWND hwnd){
	RECT ws;
	if(GetClientRect(hwnd, &ws )==FALSE){
		printf("miss to copy?\n");
		ws.top = ws.left = 0;
		ws.bottom = ws.right = 100;
	}
	
	HDC whdc = GetDC(hwnd);
		
	HBITMAP hbp = CreateCompatibleBitmap(whdc, ws.right, ws.bottom);
	HDC bmphdc = CreateCompatibleDC(whdc);
	
	
	SelectObject(bmphdc , hbp);
	BitBlt(bmphdc,0,0,ws.right,ws.bottom,whdc,0,0,SRCCOPY);
	ReleaseDC(hwnd,whdc);
	DeleteObject(hbp);
	return bmphdc;
}







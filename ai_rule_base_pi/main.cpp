#include "debug_window.h"
#include "rensya_window.h"
#include "ai.h"
#include "send_key.h"
#include "define.h"

#include<cstdio>
#include<cstring>
#include<vector>
#include<queue>
#include<algorithm>
#include<cmath>
#include<climits>
#include<string>
#include<set>
#include<map>
using namespace std;
#define rep(i,n) for(int i=0;i<((int)(n));i++)
#define reg(i,a,b) for(int i=((int)(a));i<=((int)(b));i++)
#define irep(i,n) for(int i=((int)(n))-1;i>=0;i--)
#define ireg(i,a,b) for(int i=((int)(b));i>=((int)(a));i--)
typedef long long int lli;
typedef pair<int,int> mp;
#define fir first
#define sec second
#define IINF INT_MAX
#define LINF LLONG_MAX

#include <Windows.h>
#include <cstdio>
#include<ctime>

#define eprintf(...)  fprintf(stderr,__VA_ARGS__)



HWND GetHwnd(const char* wname){
	HWND res = GetDesktopWindow();;
	res = FindWindowEx(res, NULL, NULL, wname);
	return res;
}


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow ){
	
	
	HWND hwnd=NULL;
#ifdef ONLINE
	const char browsername[400]="T-Rex Game - Google Chrome";
#else
	//const char browsername[400]="http://お手元のスマホでもプレイできます/ に接続できません - Google Chrome";
	const char browsername[400]="http://www.tsg.ne.jp/ に接続できません - Google Chrome";
	//ここを、適当な文字列にしてください。
	//例えば、googleにつなげようとしてつながらないときは、
	//const char browsername[400]="http://www.google.co.jp/ に接続できません - Google Chrome";
	//にしてみてください。(まあ、端的に言ってウインドウ名です。)
#endif

	hwnd = GetHwnd(browsername);
	if(hwnd==NULL){
		eprintf("%s mode\n",
#ifdef ONLINE
			"online"
#else
			"offline"
#endif
		);
		eprintf("cantfind chrome\n");
		return 0;
	}
	

	RECT ws;
	if(GetClientRect(hwnd, &ws )==FALSE){
		ws.top = ws.left = 0;
		ws.bottom = ws.right = 100;
	}
	eprintf("width .. %ld height .. %ld\n",ws.right,ws.bottom);
	//return 0;
	SetWindowPos(hwnd,NULL,0,0,726,562,SWP_NOMOVE | SWP_NOZORDER);
	
	
	HWND debhwnd = makewindow(hInstance);
	if(debhwnd==NULL){
		eprintf("cantfind debugwindow\n");
		return -1;
	}
	
	SetForegroundWindow(debhwnd);
	SetForegroundWindow(hwnd);
	init_send_key(hwnd);

	MSG msg;
	
	class_ai* ai = new class_ai();
	
	SetTimer(debhwnd , 1 , 1 , NULL); 
	
	clock_t mt = clock();
	clock_t terv = clock();
	eprintf("start\n");
	while(GetMessage(&msg , NULL , 0 , 0)){
		DispatchMessage(&msg);
		
		if((((double)(clock()-mt))/CLOCKS_PER_SEC) * (10000.0)>(1.0)){ //judgeの間隔です。
			HDC ghdc = CopyWindow(hwnd);
			
			mt = clock(); 
			if(ghdc==NULL){
				eprintf("ghdc is null!!\n");
				hwnd = GetHwnd(browsername);
				if(hwnd==NULL){
					eprintf("can't find chrome\n");
					exit(-1);
				}
				eprintf("gethwnd %d\n",(int)hwnd);
			}
			ai->ai_decision(ghdc,mt); //画像を取った時間も一緒に渡してやる
			
			SendMessage(debhwnd,WM_APP,0,((LPARAM)ghdc));
			mt = terv = clock();
		}
	}
	
	delete ai;
	
	return msg.wParam;
}


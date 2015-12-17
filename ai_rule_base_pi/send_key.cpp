#include "send_key.h"

#include<cstdio>
#include<ctime>

#define rep(i,n) for(int i=0;i<((int)(n));i++)

#define eprintf(...)  fprintf(stderr,__VA_ARGS__)

#define KEY_SPACE 2


HWND mhwnd=NULL;
bool mst[3];


void init_send_key(HWND hwnd){
	mhwnd=hwnd;
	rep(i,3)mst[i]=false;
}

keystate::keystate(){
	rep(i,3)st[i]=false;
}

int key_to_vkey(int k){
	switch(k){
	case KEY_UP:
		return VK_UP;
	case KEY_DOWN:
		return VK_DOWN;
	case KEY_SPACE:
		return VK_SPACE;
	}
	return -1;
}

void send_keystate(keystate& keys){
	if(mhwnd==NULL){
		fprintf(stderr,"%s mhwnd_is_null\n",__FILE__);
		exit(-1);
	}
	rep(i,3){
		if((!mst[i]) && keys.st[i]){
			SendMessage(mhwnd,WM_KEYDOWN,key_to_vkey(i),0);
		}
		else if(mst[i] && (!keys.st[i])){
			SendMessage(mhwnd,WM_KEYUP,key_to_vkey(i),0);
		}
		mst[i]=keys.st[i];
	}
}

void push_space(){
	SendMessage(mhwnd,WM_KEYDOWN,VK_SPACE,0);
	SendMessage(mhwnd,WM_KEYUP,VK_SPACE,0);
}

void push_jump(){
	SendMessage(mhwnd,WM_KEYDOWN,VK_UP,0);
	SendMessage(mhwnd,WM_KEYUP,VK_UP,0);
}



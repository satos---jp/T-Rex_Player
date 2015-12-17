#ifndef SEND_KEY
#define SEND_KEY

#include<windows.h>
void init_send_key(HWND hwnd);

struct keystate{
	bool st[3];
	keystate();
};

#define KEY_UP 0
#define KEY_DOWN 1

//space‚Í’·‰Ÿ‚µ‚³‚¹‚È‚¢B

void send_keystate(keystate& keys);

void push_space();
void push_jump();

#endif
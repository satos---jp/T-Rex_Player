#ifndef JUDGER_H
#define JUDGER_H
#include <Windows.h>

#include "define.h"
#include<vector>
using namespace std;

struct box{
	int u,d,l,r;
	box(){u=d=l=r=-1;}
};

//bool invest_and_draw(HDC& hdc,int dat[][JUDGE_X],pos& hanap);
//bool ishanadown(HDC& hdc);
int invest_and_draw(HDC& hdc,vector<box> &bx,pos& hanap,box& dbox,SYSTEMTIME stgltime); 
//hanap�́A�O��̂𑗂��Ă��ƌv�Z���x�������B
//over��-1,���Ⴊ�݂�0,�ӂ���1,���ł�2��Ԃ�


#endif
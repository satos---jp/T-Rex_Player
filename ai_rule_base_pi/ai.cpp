#include "ai.h"
#include "judger.h"
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

#define eprintf(...)  fprintf(stderr,__VA_ARGS__)



void class_ai::init(){
	hnp = pos();
	mbs = vector<box>();
	pred_speed = 0;
	startt = memt;
	memt = -1;
	hanamh =-1;
	jping=false;
	
    GetLocalTime(&start_globe_t);
}



class_ai::class_ai(){
	init();
	startt=clock();
}



void class_ai::ai_decision(HDC& hdc,clock_t capt){
	
	vector<box> nbs;
	box db;
	int state = invest_and_draw(hdc,nbs,hnp,db,start_globe_t);
	
	
	if(memt<0); //時刻の初期化タイミングが必要。
	else if(mbs.size()==0 || nbs.size()==0);
	else{
		box bbx = mbs[0];
		box nbx = nbs[0];
		
		double nd;
		if(nbx.l<bbx.l){
			if(nbx.l>=10)nd=bbx.l-nbx.l; //左で測る
			else nd=bbx.r-nbx.r;//左端が見切れている可能性がある。ので、後ろで測る。

			nd /= (((double)(capt-memt))/CLOCKS_PER_SEC);

			if(pred_speed<0.1)pred_speed=nd; //最初なので。
			else pred_speed += 0.03*(nd - pred_speed);
		}
	}
	
	
	
	bool jp=false;
	bool dw=false;
	
	switch(state){
	case 2: //ジャンプ中
		if(nbs.size()==0 || nbs[0].r - db.l < pred_speed/30){
			//沈んで大丈夫。
			dw=true;
		}
		if(jping && hnp.y <= hanamh){ //鼻の値が減っている -> 鼻が上がっている->上昇中とみなせる。
			if(!dw){ //下降中はもちろんいらない。
				jp=true; //上押しとく。
			}
		}
		else jping=false;
		break;
	case 1: //通常
		if(nbs.size()!=0){
			if(nbs[0].r<db.l + 5)break; //左端のやつを認識していたっぽい？ので。
			int dinop = (db.l+db.r)/2;
			int obsp = (nbs[0].l+nbs[0].r)/2;
			if(obsp - (dinop + pred_speed/4.5) <5){ //跳べ！ 
				//下入ってる→上入れる→下切れるだと、上が入らないっぽい。
				push_jump();
				jping=true;
			}
		}
		break;
	case 0: //しゃがみ
		if(nbs.size()!=0 && nbs[0].d<130){ //しゃがんどけ。
			dw=true;
		}
		break;
	case -1: //gameover
		{
			double seemtot = ((double)(capt-startt))/CLOCKS_PER_SEC;
			if(seemtot > 0.5){
				printf("dead. %f\n",seemtot);
			}
			init();
			push_space();
		}	
	}
	
	keystate ksta;

	ksta.st[KEY_UP]=jp;
	ksta.st[KEY_DOWN]=dw;
	
	if(state!=-1){
		printf("pred_interval %f ",((double)(capt-memt))/CLOCKS_PER_SEC);
		printf("pred_speed %f\n",pred_speed);
		printf("hanapos %d %d\n",hnp.x,hnp.y);
		
		if(jp)printf("jump↑\n");
		if(dw)printf("down↓\n");
	}
	send_keystate(ksta);

	mbs=nbs;
	memt=capt;	
	hanamh = hnp.y;
}




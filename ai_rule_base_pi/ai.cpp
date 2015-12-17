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
	
	
	if(memt<0); //�����̏������^�C�~���O���K�v�B
	else if(mbs.size()==0 || nbs.size()==0);
	else{
		box bbx = mbs[0];
		box nbx = nbs[0];
		
		double nd;
		if(nbx.l<bbx.l){
			if(nbx.l>=10)nd=bbx.l-nbx.l; //���ő���
			else nd=bbx.r-nbx.r;//���[�����؂�Ă���\��������B�̂ŁA���ő���B

			nd /= (((double)(capt-memt))/CLOCKS_PER_SEC);

			if(pred_speed<0.1)pred_speed=nd; //�ŏ��Ȃ̂ŁB
			else pred_speed += 0.03*(nd - pred_speed);
		}
	}
	
	
	
	bool jp=false;
	bool dw=false;
	
	switch(state){
	case 2: //�W�����v��
		if(nbs.size()==0 || nbs[0].r - db.l < pred_speed/30){
			//����ő��v�B
			dw=true;
		}
		if(jping && hnp.y <= hanamh){ //�@�̒l�������Ă��� -> �@���オ���Ă���->�㏸���Ƃ݂Ȃ���B
			if(!dw){ //���~���͂�����񂢂�Ȃ��B
				jp=true; //�㉟���Ƃ��B
			}
		}
		else jping=false;
		break;
	case 1: //�ʏ�
		if(nbs.size()!=0){
			if(nbs[0].r<db.l + 5)break; //���[�̂��F�����Ă������ۂ��H�̂ŁB
			int dinop = (db.l+db.r)/2;
			int obsp = (nbs[0].l+nbs[0].r)/2;
			if(obsp - (dinop + pred_speed/4.5) <5){ //���ׁI 
				//�������Ă遨�����遨���؂�邾�ƁA�オ����Ȃ����ۂ��B
				push_jump();
				jping=true;
			}
		}
		break;
	case 0: //���Ⴊ��
		if(nbs.size()!=0 && nbs[0].d<130){ //���Ⴊ��ǂ��B
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
		
		if(jp)printf("jump��\n");
		if(dw)printf("down��\n");
	}
	send_keystate(ksta);

	mbs=nbs;
	memt=capt;	
	hanamh = hnp.y;
}




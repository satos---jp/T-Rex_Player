#include "judger.h"
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


void drawbox(HDC& hdc,int u,int d,int r,int l){
	MoveToEx(hdc , l, u  , NULL);
	LineTo(hdc , r , u);
	LineTo(hdc , r , d);
	LineTo(hdc , l , d);
	LineTo(hdc , l , u);
}

void drawrect(HDC& hdc,int u,int d,int r,int l){
	Rectangle(hdc,l,u,r,d);
}



const int boxw=600,boxh=150;

#ifdef ONLINE
const int up=267,lp=60; //onlineの
#else
const int up=132,lp=10; //offlineの
#endif

//mokuhyouは、lpが+4,upが+3。してる。

bool isgameover(HDC& hdc){
	int overposs[8][2]={ //相対位置。
		{214,58},{238,55},{262,55},{279,55},
		{322,55},{346,53},{363,55},{395,54}
	};

	rep(i,8){
		int nx=overposs[i][0] + lp,
			ny=overposs[i][1] + up;
		int r=3;
		Arc(hdc , nx-r , ny-r , nx+r , ny+r , nx , ny-r , nx , ny-r);
		int nc=GetPixel(hdc,nx,ny);
		if(nc!=5460819)return false;
	}
	return true;
}



bool isblack(HDC& hdc,pos p){ //黒かどうか
	return (GetPixel(hdc,p.x,p.y)==5460819);
}

int meqs[200][250]; //boxh=150のときにしてます。

const int hanamarker[4][4]={ //鼻マーカー	
	{1,1,1,1},
	{1,0,0,1},
	{1,0,0,1},
	{1,1,1,1},
};


bool ishana(HDC& hdc,pos p){ //鼻たるかどうか	
	int x=p.x,y=p.y;
	if(x<0 || y<0 || y>boxh-5)return false;
	rep(dy,4){
		rep(dx,4){
			int ny=y+dy,nx=x+dx;
			pos np; np.x = nx + lp; np.y = ny + up;
			if(meqs[ny][nx]==-1)meqs[ny][nx]=(isblack(hdc,np)?1:0);
			if(hanamarker[dy][dx]!=meqs[ny][nx])return false;
		}
	}
	return true;
}

bool gethanapos(HDC& hdc,pos& p){ //よく考えたら、鼻じゃなくて目だよなあ。
	
	//2*2の空白が、
	//**
	//**
	//となってるとき、鼻とみる
	
	int x=p.x,y=p.y;
	
	bool res=false;
	
	if(x<0 || y<0){
		if(x!=-123)memset(meqs,-1,sizeof(meqs));//これが自己再帰時にいりそう。

		//新規
		rep(ny,boxh){
			rep(nx,250){
				pos np;
				np.x=nx; np.y=ny;
				if(ishana(hdc,np)){
					p.y = ny;
					p.x = nx;
					res=true;
					goto gethanapos_exit;
				}
			}
		}
		eprintf("can't find hana\n");
	}
	else{
		if(ishana(hdc,p)){
			res=true;
			goto gethanapos_exit;
		}
		reg(dy,0,100){ //跳ぶときに、100ぐらいずれる
			for(int ddy = -1; ddy<=1; ddy+=2){
				reg(dx,-30,30){ //前傾したときに、25くらいずれる。
					pos np;
					np.y=dy*ddy+y;
					np.x=dx+x;
					if(ishana(hdc,np)){
						p.y=np.y;
						p.x=np.x;
						res=true;
						goto gethanapos_exit;
					}
				}
			}
		}
		p.x=p.y=-123; //もいっぺん探しなおし
		res = gethanapos(hdc,p);
	}
	
gethanapos_exit:;
	memset(meqs,-1,sizeof(meqs));
	return res;
}



const int jdx=32; //見点の羅列がはじまるとこの、左端の位置/xbun。
const int xbun=3; //横の解像度
const int ybun=20; //縦の解像度




//鼻の位置は、
//普通...105
//低い...123
//と思われる。(これはずれないはず)




int dat[6][400];
const int checky[2]={105,125};

void drawcircle(HDC& hdc,pos p,int r){
	Arc(hdc , p.x-r , p.y-r , p.x+r , p.y+r , p.x , p.y-r , p.x , p.y-r);
}

const int dinold = -24;
const int dinord =  17;
const int dinodd =  41;


vector<box> getobss(HDC& hdc,box dst,int hh){ 
	//dstとして恐竜の状態、hhとして鼻の状態を入れると、障害物の位置、高さを、返す
	//hhはしゃがみで0,ふつうで1,飛んでて2で。
	
	vector<box> resbox;
	
	
	memset(dat,0,sizeof(dat));
	rep(y,2){
		rep(x,200){
			pos np;
			np.x = x*xbun+lp;
			np.y = checky[y]+up;
			
			bool ib=isblack(hdc,np);
			//drawcircle(hdc,np,1);
			dat[y][x]=(ib?1:0);
		}
	}
	
	//とりあえず、サボテンを確実に判別する。
	
	//たかだか、10くらいでよい？


	//kaisir
	rep(x,200){
		rep(y,2){
			int cdnx=x*xbun;
			if(cdnx < dst.r+20){
			//8どころじゃたりないことが判明したので、(バッファがだぶる)
			//たとえば、20とかでどですか。
				//立ってると、0,1が。しゃがんでると、1が。かかる。
				if(hh==0){ //しゃがみ
					if(y==1)continue;
				}
				else{
					if(dst.d+6 > checky[y] )continue; //下が低ければ自分の。
					
				}
			}
			
			if(dat[y][x]==1){
				int u=y,d=y;
				if(y==0 && dat[1][x]==1 && hh!=0)d=1;
				int l=x,r=x;

				int ct=12; //これくらいいる。で十分やろ。
				while(ct>0){
					r++;
					if(dat[d][r]!=0 || dat[u][r]!=0)ct=12;
					else ct--;
				}
				
				r-=12;
				//あと、l,rの近傍を調べる外に8,上下+-3で。
				int mr=r*xbun+lp, ml=l*xbun+lp;
				int resr=mr, resl=ml;
				
				{
					rep(qqq,2){
						int mu=checky[u];
						if(qqq>0){
							if(u==d)continue;
							mu=checky[d];
						}
						mu += up;
						rep(dx,8){ 
							reg(dy,-5,5){
								pos cp;
								cp.x = mr+dx;
								cp.y = mu+dy;
								if(isblack(hdc,cp))resr=max(resr,cp.x);
								cp.x = ml-dx;
								cp.y = mu+dy;
								if(isblack(hdc,cp))resl=min(resl,cp.x);
							}
						}
					}
				}
				
				
				box cb;
				cb.l = resl - lp;
				cb.r = resr - lp;
				
				/*
				if(u==1)cb.u=113; //小さい木の頭
				else{    
					if(d==1)cb.u=98; //大きい木の頭
					else cb.u=200; //飛ぶ鳥の頭
				}*/
				//めんどいので、高いほうでとっておきます。
				cb.u=98;
				
				if(d==1)cb.d=145;
				else cb.d=110; //鳥脚
				
				resbox.push_back(cb);
				x=r+1;
				break;
			}
		}
	}
	

	return resbox;
}
			
	

//鼻の位置は、
//普通...105
//低い...123
//と思われる。(これはずれないはず)


int invest_and_draw(HDC& hdc,vector<box> &bx,pos& hanap,box& dbox,SYSTEMTIME stgltime){ 
	//overは-1,しゃがみで0,ふつうで1,飛んでて2を返す
	
	bool nover=isgameover(hdc);
	bx = vector<box>();
	
	int res=-1;
	
	dbox=box();
	
	if(!nover){
		bool gh = gethanapos(hdc,hanap);
		
		
		if(gh){	
			
			if(hanap.y>=120)res=0;
			else if(hanap.y<=100)res=2; //ちっと、基準をゆるめるか(103->100)
			else res=1;
			
			if(res!=0){ //恐竜の箱としての位置
				dbox.u = hanap.y-3;
				dbox.d = hanap.y + dinodd;
				dbox.l = hanap.x + dinold;
				dbox.r = hanap.x + dinord;
			}
			else{
				dbox.u = hanap.y-4;
				dbox.d = hanap.y+23;
				dbox.l = hanap.x-39;
				dbox.r = hanap.x + dinord;
			} 

			bx = getobss(hdc,dbox,res); //鼻から、全部相対でやってく。
			
		}
	}
	
	HBRUSH redbrush = CreateSolidBrush(RGB(255,0,0)); //1
	HPEN greenpen = CreatePen(PS_SOLID , 0 ,RGB(0,255,0)); //2
	HPEN mazepen = CreatePen(PS_SOLID , 0 ,RGB(255,0,255)); //3
	HBRUSH bluebrush = CreateSolidBrush(RGB(0,0,255)); //4
	

	COLORREF wakucol;
	if(nover)wakucol=RGB(255,0,0); //真っ赤
	else{
		if(res==1)wakucol = RGB(0,0,128); //通常青
		else if(res==2)wakucol = RGB(0,255,0); //飛んでると緑
		else wakucol = RGB(255,255,0); //しゃがむと黄
	}
	
	HBRUSH backbrush = CreateSolidBrush(RGB(64,64,64)); //5
	
	SelectObject(hdc , backbrush);
	
	drawrect(hdc,up,up+boxh,lp,lp+boxw); //背景
	
	
	HPEN wakupen = CreatePen(PS_SOLID , 0 ,wakucol); //6
	SelectObject(hdc , wakupen);
	drawbox(hdc,up,up+boxh,lp,lp+boxw); //外枠
	MoveToEx(hdc , lp, up+139, NULL); //地平線
	LineTo(hdc , lp+boxw , up+139);
	
	
	SelectObject(hdc , bluebrush);
	drawrect(hdc, up+dbox.u, up+dbox.d, lp+dbox.l, lp+dbox.r);
	
	
	SelectObject(hdc , mazepen);
	pos cp;
	cp.x=hanap.x+lp,cp.y=hanap.y+up;
	drawcircle(hdc,cp,3);
	

	SelectObject(hdc , redbrush);
	rep(i,bx.size()){
		box nb = bx[i];
		drawrect(hdc,nb.u+up ,nb.d+up ,nb.l+lp ,nb.r+lp);
	}

	
	
	
	
	DeleteObject(wakupen);
	DeleteObject(redbrush);
	DeleteObject(bluebrush);
	DeleteObject(greenpen);
	DeleteObject(mazepen);
	DeleteObject(backbrush); 
	

	/*
	
	HFONT hfont = CreateFont(
			40 , 0 , 0 , 0 , FW_BOLD , FALSE , FALSE , FALSE ,
			SHIFTJIS_CHARSET , OUT_DEFAULT_PRECIS ,
			CLIP_DEFAULT_PRECIS , DEFAULT_QUALITY , 
			DEFAULT_PITCH | FF_ROMAN , TEXT("メイリオ レギュラー 日本語")
		);	

	SelectObject(hdc,hfont);
	
	char ns[105];
	sprintf(ns,"%d.%02d.%02d %2d:%02d:%02d   より継続中                 ", 
		stgltime.wYear, stgltime.wMonth, stgltime.wDay, 
		stgltime.wHour, stgltime.wMinute, stgltime.wSecond);
	
	TextOut(hdc,0,0,ns,strlen(ns));
	
	sprintf(ns,"AIが自動でゲームをしています。                    ");
	TextOut(hdc,0,80,ns,strlen(ns));
	
	sprintf(ns,"これはAIが下の画面を認識したものです。");
	TextOut(hdc,0,40,ns,strlen(ns));
	

	DeleteObject(hfont);
	*/
	
	return res;
}




















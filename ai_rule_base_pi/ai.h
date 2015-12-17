#ifndef AI_H
#define AI_H

#include <Windows.h>
#include <ctime>
#include "judger.h"

class class_ai{
public:
	void ai_decision(HDC& hdc,clock_t capt);
	class_ai();
	
private:
	double pred_speed; //予測速度
	pos hnp; //前回のboxの位置を覚えて、差分から速度を出す。
	vector<box>  mbs; //前回のboxの位置を覚えて、差分から速度を出す。
	void init();
	clock_t memt; //前回のclockを覚えて、差分から速度を出す。
	int hanamh; //前回の鼻の高さを覚えて、差分から上下降を出す。
	
	bool jping; //ジャンプ時にフラグを立て、一回でも下降していたら消す。
	
	clock_t startt; //開始時からの累積をとれるようにする
	
    SYSTEMTIME start_globe_t; //開始時刻が出たらよくないすか？
};

#endif
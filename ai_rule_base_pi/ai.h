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
	double pred_speed; //�\�����x
	pos hnp; //�O���box�̈ʒu���o���āA�������瑬�x���o���B
	vector<box>  mbs; //�O���box�̈ʒu���o���āA�������瑬�x���o���B
	void init();
	clock_t memt; //�O���clock���o���āA�������瑬�x���o���B
	int hanamh; //�O��̕@�̍������o���āA��������㉺�~���o���B
	
	bool jping; //�W�����v���Ƀt���O�𗧂āA���ł����~���Ă���������B
	
	clock_t startt; //�J�n������̗ݐς��Ƃ��悤�ɂ���
	
    SYSTEMTIME start_globe_t; //�J�n�������o����悭�Ȃ������H
};

#endif
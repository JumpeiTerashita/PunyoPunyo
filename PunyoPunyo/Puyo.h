#pragma once

#pragma once

#include "Sequence.h"
#include "GameObject.h"




class Puyo :public GameObject, public Sequence<Puyo>
{

	
	//TODO public private protected 分ける
public:
	//void CreatePuyo();
	Puyo() {};
	Puyo(int _x, int _y,int _colors);

	Puyo(int _x, int _y, STATE _seq, COLORPATTERN _colorNumber);
	~Puyo() {};
	
	
	
	float size;
	bool _is_checked;
	bool _is_rotate;
	bool _will_delete;
	


	void delMap(int _x,int _y);
	void map(int _x,int _y,STATE _seq);

	//TODO ColorSetup　ダサい！！！

	float colorStatus[4];
	void ColorSetup(COLORPATTERN _colorNumber);
	
	void Fall();

	//TODO turn　回しながらひっかけるとちぎれる
	void TurnCounterClockwise();
	void TurnClockwise();

	void Set();
	void FreeFall();

	
	
	bool UnderCollision();

	void ObjUpdate();
	void ObjDisp();
	bool Search_There_is(int _x, int _y);
	bool Search_is_Falling(int _x, int _y);
private:
	
	

};




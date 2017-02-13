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
	


	void delMap(int _x,int _y);
	void map(int _x,int _y,STATE _seq);

	//TODO ColorSetup　ダサい！！！

	float colorStatus[4];
	void ColorSetup(COLORPATTERN _colorNumber);
	
	void Fall();
	void InputMove();
	void Turn();
	void Set();
	void FreeFall();

	
	
	bool UnderCollision();

	void ObjUpdate();
	void ObjDisp();

private:
	bool Search_There_is(int _x, int _y);
	bool Search_is_Falling(int _x, int _y);
	bool Keyflag_left;
	bool Keyflag_right;

};




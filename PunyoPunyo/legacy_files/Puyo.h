#pragma once

#include "GameObject.h"

enum SEQUENCE
{
	STATE_FALL,
	STATE_FREEFALL,
	STATE_TOUCHDOWN
};

class Puyo:public GameObject
{
public:
	Puyo(){};
	Puyo(int _x,int _y);
	Puyo(SEQUENCE _sequence);
	~Puyo() {};
	int FallTimer;
	
	float size;
	bool _is_checked;
	SEQUENCE sequence;

	float colorStatus[4];
	void ColorSetup(COLORPATTERN _colorNumber);


	void ObjUpdate();
	void ObjDisp();


};




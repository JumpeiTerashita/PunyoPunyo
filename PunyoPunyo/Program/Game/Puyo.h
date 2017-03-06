#pragma once

#include "../Liblary/Sequence.h"
#include "../Engine/GameObject.h"

enum COLORPATTERN
{
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_PURPLE,
	COLOR_LIGHTBLUE,
	COLOR_ORANGE,
	COLOR_NULL
};

enum STATUS
{
	IS_CHECKED,
	IS_ROTATE,
	IS_FALLING,
	IS_FREEFALL,
	WILL_DELETE
};

class Puyo :public GameObject, public Sequence<Puyo>
{
public:

	Puyo() {};
	Puyo(int _x, int _y,int _Colors);
	Puyo(int _x, int _y, COLORPATTERN _SetColor);
	~Puyo() {};
	COLORPATTERN ColorNumber;
	unsigned char Status; //Ç’ÇÊèÛë‘ÉtÉâÉOÅ@(enum STATUS)
	
	void Fall();
	void FreeFall();
	
	void ObjUpdate();
	void ObjDisp();

	float colorStatus[4];
	void ColorSetup(COLORPATTERN _colorNumber);
	
private:
	bool Search_There_is(int _x, int _y);
	bool Search_is_Falling(int _x, int _y);
	
	void TurnCounterClockwise();
	void TurnClockwise();
	bool UnderCollision();

	
};

extern unsigned long genrand_int32(void);


#pragma once

#pragma once

#include "../Liblary/Sequence.h"
#include "../Engine/GameObject.h"

enum COLORPATTERN
{
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_MAX
};

enum STATUS
{
	IS_CHECKED,
	IS_ROTATE,
	WILL_DELETE
};

class Puyo :public GameObject, public Sequence<Puyo>
{
public:

	Puyo() {};
	Puyo(int _x, int _y,int _colors);
	Puyo(int _x, int _y, COLORPATTERN _setColor);
	~Puyo() {};
	COLORPATTERN ColorNumber;
	unsigned char Status;
	
	bool _is_checked;
	bool _is_rotate;
	bool _is_falling;
	bool _is_freefall;
	bool _will_delete;

	void Fall();
	void FreeFall();
	
	void ObjUpdate();
	void ObjDisp();

	float colorStatus[4];
	void ColorSetup(COLORPATTERN _colorNumber);
private:
	void delMap(int _x, int _y);
	void map(int _x, int _y, COLORPATTERN _setColor);
	bool Search_There_is(int _x, int _y);
	bool Search_is_Falling(int _x, int _y);
	//TODO ColorSetup　ダサい！！！
	
	void TurnCounterClockwise();
	void TurnClockwise();
	bool UnderCollision();


};




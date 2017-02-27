#pragma once

#include "../Liblary/Sequence.h"
#include "../Engine/GameObject.h"

enum COLORPATTERN_BATTLE
{
	RED,
	GREEN,
	BLUE,
	YELLOW,
	OJAMA,
	MAX
};

enum STATUS_BATTLE
{
	CHECKED,
	ROTATE,
	WILL_DEL
};

class BattlePuyo :public GameObject, public Sequence<BattlePuyo>
{
public:

	BattlePuyo() {};
	BattlePuyo(int _x, int _y, int _colors);
	BattlePuyo(int _x, int _y, COLORPATTERN_BATTLE _setColor);
	~BattlePuyo() {};
	COLORPATTERN_BATTLE ColorNumber;
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
	void ColorSetup(COLORPATTERN_BATTLE _colorNumber);
	
private:
	void delMap(int _x, int _y);
	void map(int _x, int _y, COLORPATTERN_BATTLE _setColor);
	bool Search_There_is(int _x, int _y);
	bool Search_is_Falling(int _x, int _y);
	//TODO ColorSetup　ダサい！！！

	void TurnCounterClockwise();
	void TurnClockwise();
	bool UnderCollision();


};





#include <time.h>
#include <stdlib.h>
#include "../glut.h"
#include "Puyo_Battle.h"
#include "Scene_Battle.h"
#include "GameManager.h"
#include "../Engine/InputManager.h"

BattlePuyo::BattlePuyo(int _x, int _y, int _colors)
{
	setLifeTime(1);
	Status = 0;
	_is_falling = true;
	_is_freefall = false;
	_is_checked = false;
	_is_rotate = false;
	_will_delete = false;
	pos.set(_x, _y);
	ColorNumber = (COLORPATTERN_BATTLE)(rand() % _colors);
	ColorSetup(ColorNumber);
	setSequence(&BattlePuyo::Fall);

}

BattlePuyo::BattlePuyo(int _x, int _y, COLORPATTERN_BATTLE _setColor)
{
	setLifeTime(1);
	_is_falling = _is_falling;
	_is_freefall = _is_freefall;
	_is_rotate = _is_rotate;
	_is_checked = false;
	_will_delete = false;
	pos.set(_x, _y);
	ColorNumber = _setColor;
	ColorSetup(_setColor);
}

void BattlePuyo::ObjUpdate()
{
	runSequence();
	setPosYPriority(pos._y);
	setPosXPriority(pos._x);
}



void BattlePuyo::delMap(int _x, int _y)
{
	SceneBattle::getInstance()->map[_y][_x] = nullptr;
}

void BattlePuyo::map(int _x, int _y, COLORPATTERN_BATTLE _setColor)
{
	SceneBattle::getInstance()->map[_y][_x] = new BattlePuyo(_x, _y, _setColor);

}


static int c = 0;

void BattlePuyo::Fall()
{

	SceneBattle* Scene = SceneBattle::getInstance();

	if (0 == c && Scene->cc == 1) {
		printf("------------------------------\n");
	}
	if (Scene->cc == 1) {
		printf("x = %d y = %d\n", pos._x, pos._y);
		/*std::string col[4] = { "��","��","��", "��" };
		printf("%s\n", col[ColorNumber].c_str());*/
		c++;
		c %= 2;
	}

	if (_is_freefall) setSequence(&BattlePuyo::FreeFall);
	//if (_state == STATE_FREEFALL) setSequence(&Puyo::FreeFall);
	unsigned char KeyFlag = Scene->KeyFlag;
	int isLeft = KeyFlag & KeyFlag_LEFT;
	int isRight = KeyFlag & KeyFlag_RIGHT;
	int isTurn_CounterClockwise = KeyFlag & KeyFlag_Turn_CounterClockwise;
	int isTurn_Clockwise = KeyFlag & KeyFlag_Turn_Clockwise;
	//if (Scene->Keyflag_left)
	if (isLeft)
	{
		if (!Search_There_is(pos._x - 1, pos._y))
		{
			if (pos._x > 1)
			{
				delMap(pos._x, pos._y);
				pos._x--;
				map(pos._x, pos._y, ColorNumber);
			}
		}
		/*else
		{
		if (Search_is_Falling(pos._x - 1, pos._y))
		{
		if (pos._x > 2 && !Search_There_is(pos._x - 2, pos._y))
		{
		delMap(pos._x, pos._y);
		pos._x--;
		map(pos._x, pos._y,ColorNumber);
		}
		}
		}*/
	}
	if (isRight)
		//if (Scene->Keyflag_right)
	{
		if (!Search_There_is(pos._x + 1, pos._y))
		{
			if (pos._x < 6)
			{
				delMap(pos._x, pos._y);
				pos._x++;
				map(pos._x, pos._y, ColorNumber);
			}
		}
		/*else
		{
		if (Search_is_Falling(pos._x + 1, pos._y))
		{
		if (pos._x < 5 && !Search_There_is(pos._x + 2, pos._y))
		{
		delMap(pos._x, pos._y);
		pos._x++;
		map(pos._x, pos._y,ColorNumber);
		}
		}
		}*/
	}

	//if (Scene->Keyflag_turnCounterClockwise)	
	if (isTurn_CounterClockwise) TurnCounterClockwise();
	if (isTurn_Clockwise) TurnClockwise();
	//if (Scene->Keyflag_turnClockwise)	TurnClockwise();

	if (getFrame() >= Scene->FallLimit)
	{

		if (!UnderCollision())
		{
			delMap(pos._x, pos._y);
			pos._y--;
			setFrame(0);
			map(pos._x, pos._y, ColorNumber);
		}

	}




}



void BattlePuyo::TurnCounterClockwise()
{
	
	SceneBattle* Scene = SceneBattle::getInstance();
	if (_is_rotate)
	{
		if (Search_There_is(pos._x, pos._y + 1))
		{
			if (!Search_There_is(pos._x + 1, pos._y + 1) && pos._x < 6)
			{
				delMap(pos._x, pos._y);
				pos._x++;
				pos._y++;
				map(pos._x, pos._y, ColorNumber);
				Scene->_is_vertical = false;
				return;
			}
		}
		else if (Search_There_is(pos._x - 1, pos._y))
		{
			if (!Search_There_is(pos._x - 1, pos._y + 1))
			{
				delMap(pos._x, pos._y);
				pos._x--;
				pos._y++;
				map(pos._x, pos._y, ColorNumber);
				Scene->_is_vertical = true;
				return;
			}
		}
		else if (Search_There_is(pos._x, pos._y - 1))
		{
			if (!Search_There_is(pos._x - 1, pos._y - 1) && pos._x > 1)
			{
				delMap(pos._x, pos._y);
				pos._x--;
				pos._y--;
				map(pos._x, pos._y, ColorNumber);
				Scene->_is_vertical = false;
				return;
			}
		}
		else if (Search_There_is(pos._x + 1, pos._y))
		{
			if (!Search_There_is(pos._x + 1, pos._y - 1) && pos._y>1)
			{
				delMap(pos._x, pos._y);
				pos._x++;
				pos._y--;
				map(pos._x, pos._y, ColorNumber);
				Scene->_is_vertical = true;
				return;
			}
		}
		Scene->KeyFlag = 0;
	}
}

void BattlePuyo::TurnClockwise()
{
	SceneBattle* Scene = SceneBattle::getInstance();
	if (_is_rotate)
	{
		if (Search_There_is(pos._x, pos._y + 1))
		{
			if (!Search_There_is(pos._x - 1, pos._y + 1) && pos._x > 1)
			{
				delMap(pos._x, pos._y);
				pos._x--;
				pos._y++;
				map(pos._x, pos._y, ColorNumber);
				Scene->_is_vertical = false;
				return;
			}
		}
		else if (Search_There_is(pos._x + 1, pos._y))
		{
			if (!Search_There_is(pos._x + 1, pos._y + 1))
			{
				delMap(pos._x, pos._y);
				pos._x++;
				pos._y++;
				map(pos._x, pos._y, ColorNumber);
				Scene->_is_vertical = true;
				return;
			}
		}
		else if (Search_There_is(pos._x, pos._y - 1))
		{
			if (!Search_There_is(pos._x + 1, pos._y - 1) && pos._x < 6)
			{
				delMap(pos._x, pos._y);
				pos._x++;
				pos._y--;
				map(pos._x, pos._y, ColorNumber);
				Scene->_is_vertical = false;
				return;
			}
		}
		else if (Search_There_is(pos._x - 1, pos._y))
		{
			if (!Search_There_is(pos._x - 1, pos._y - 1) && pos._y>1)
			{
				delMap(pos._x, pos._y);
				pos._x--;
				pos._y--;
				map(pos._x, pos._y, ColorNumber);
				Scene->_is_vertical = true;
				return;
			}
		}
		Scene->KeyFlag = 0;
	}
}

void BattlePuyo::FreeFall()
{

	if (getFrame() >= 5)
	{
		if (!UnderCollision())
		{
			delMap(pos._x, pos._y);
			pos._y--;
			setFrame(0);
			map(pos._x, pos._y, ColorNumber);
		}

	}
}


bool BattlePuyo::UnderCollision()
{
	SceneBattle* Scene = SceneBattle::getInstance();
	if (pos._y == 1)
	{

		_is_falling = false;
		_is_freefall = false;
		map(pos._x, pos._y, ColorNumber);
		setLifeTime(0);
		return true;
	}

	if (Search_There_is(pos._x, pos._y - 1) && !Search_is_Falling(pos._x, pos._y - 1))
	{

		_is_falling = false;
		_is_freefall = false;
		map(pos._x, pos._y, ColorNumber);
		setLifeTime(0);
		return true;
	}
	else if (Search_is_Falling(pos._x, pos._y - 1))
	{
		if (pos._y == 2 || Search_There_is(pos._x, pos._y - 2))
		{

			_is_falling = false;
			_is_freefall = false;
			map(pos._x, pos._y, ColorNumber);
			setLifeTime(0);
			return true;
		}
	}
	return false;
}

bool BattlePuyo::Search_is_Falling(int _x, int _y)
{
	if (SceneBattle::getInstance()->map[_y][_x] == nullptr) return false;
	else if (SceneBattle::getInstance()->map[_y][_x]->_is_falling) return true;
	return false;
}

bool BattlePuyo::Search_There_is(int _x, int _y)
{
	if (SceneBattle::getInstance()->map[_y][_x] == nullptr) return false;
	return true;
}


void BattlePuyo::ObjDisp()
{
	if (!Search_There_is(pos._x, pos._y))
	{
		glPushMatrix();
		{
			glColor4f(colorStatus[0], colorStatus[1], colorStatus[2], colorStatus[3]);
			glTranslatef(-0.7 + (pos._x - 1)*0.15, -0.75 + (pos._y - 1)*0.15, 1);
			glutSolidSphere(0.075, 180, 5);
		}
		glPopMatrix();
	}
	glColor4f(1, 1, 1, 1);
	setRenderPriority(pos._y);
}

void BattlePuyo::ColorSetup(COLORPATTERN_BATTLE _colorNumber)
{
	switch (_colorNumber)
	{
	case COLOR_RED:
		colorStatus[0] = 1;
		colorStatus[1] = 0;
		colorStatus[2] = 0;
		colorStatus[3] = 1;
		break;
	case COLOR_BLUE:
		colorStatus[0] = 0;
		colorStatus[1] = 0.3f;
		colorStatus[2] = 1;
		colorStatus[3] = 1;
		break;
	case COLOR_GREEN:
		colorStatus[0] = 0;
		colorStatus[1] = 1;
		colorStatus[2] = 0;
		colorStatus[3] = 1;
		break;
	case COLOR_YELLOW:
		colorStatus[0] = 1;
		colorStatus[1] = 1;
		colorStatus[2] = 0;
		colorStatus[3] = 1;
		break;
	case COLOR_OJAMA:
		colorStatus[0] = 1;
		colorStatus[1] = 1;
		colorStatus[2] = 1;
		colorStatus[3] = 1;
		break;
	default:
		break;
	}
	return;
}
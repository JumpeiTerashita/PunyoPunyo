#include <time.h>
#include <stdlib.h>
#include "glut.h"
#include "Puyo.h"
#include "Scene_InGame.h"
#include "GameManager.h"
#include "InputManager.h"

Puyo::Puyo(int _x, int _y,int _colors)
{
	setLifeTime(1);
	_state = STATE_FALL;
	_is_checked = false;

	
	pos.set(_x, _y);
	ColorNumber = (COLORPATTERN)(rand() % _colors);
	ColorSetup(ColorNumber);
	setSequence(&Puyo::Fall);

}

Puyo::Puyo(int _x, int _y, STATE _seq, COLORPATTERN _colorNumber)
{
	setLifeTime(1);
	_state = _seq;
	_is_checked = false;

	
	pos.set(_x, _y);
	ColorNumber = _colorNumber;

	ColorSetup(ColorNumber);

}

void Puyo::ObjUpdate()
{
	runSequence();
}



void Puyo::delMap(int _x, int _y)
{
	SceneIngame::getInstance()->map[_y][_x] = nullptr;
}

void Puyo::map(int _x, int _y, STATE _seq)
{
	SceneIngame::getInstance()->map[_y][_x] = new Puyo(_x, _y, _seq, ColorNumber);
}


void Puyo::Fall()
{
	SceneIngame* Scene = SceneIngame::getInstance();

	if (Keyflag_left)
	{
		if (Scene->map[pos._y][pos._x - 1] == nullptr)
		{
			if (pos._x > 1)
			{
				delMap(pos._x, pos._y);
				pos._x--;
				map(pos._x, pos._y, STATE_FALL); 
			}
		}
		else
		{
			if (Scene->map[pos._y][pos._x - 1]->_state == STATE_FALL)
			{
				if (pos._x > 2 && Scene->map[pos._y][pos._x - 2] == nullptr)
				{
					delMap(pos._x, pos._y);
					pos._x--;
					map(pos._x, pos._y, STATE_FALL);
				}
			}
		}
		addFrame(-20);
		Keyflag_left = false;
	}

	if (Keyflag_right)
	{
		if (Scene->map[pos._y][pos._x + 1] == nullptr)
		{
			if (pos._x < 6)
			{
				delMap(pos._x, pos._y);
				pos._x++;
				map(pos._x, pos._y, STATE_FALL);
			}
		}
		else
		{
			if (Scene->map[pos._y][pos._x + 1]->_state == STATE_FALL)
			{
				if (pos._x < 5 && Scene->map[pos._y][pos._x + 2] == nullptr)
				{
					delMap(pos._x, pos._y);
					pos._x++;
					map(pos._x, pos._y, STATE_FALL);
				}
			}
		}
		addFrame(-20);
		Keyflag_right = false;
		
	}


	if (getFrame() >= 30)
	{

		if (!UnderCollision())
		{
			delMap(pos._x, pos._y);
			pos._y--;
			setFrame(0);
			map(pos._x, pos._y, STATE_FALL);
		}
		else if (pos._x==3&&pos._y>=11)
		{
			if (Scene->map[pos._y - 1][pos._x] != nullptr&&Scene->map[pos._y - 1][pos._x]->_state != STATE_FALL)
			{
				Scene->GameOver = true;
			}
				
		}
		
	}

	InputMove();

	
}

void Puyo::InputMove()
{
	SceneIngame* Scene = SceneIngame::getInstance();
	int FallPuyoCount = Scene->Search_States_are(STATE_FALL);
	printf("\n%d\n", FallPuyoCount);
	if (FallPuyoCount == 1)
	{
		addFrame(15);
		return;
	}
	if (_SpecialKey == GLUT_KEY_RIGHT||_Key == 'd')
		//(input.AnalogX > 700 && getFrame() % 60 == 0)
	{
		Keyflag_right = true;
		
	}
	if (_SpecialKey == GLUT_KEY_LEFT || _Key == 'a')
	{
		Keyflag_left = true;
		
	}
	if (_Key == ' ')
	{
		Turn();
	}
	if (_SpecialKey == GLUT_KEY_DOWN||_Key=='s')
	{
		Scene->ScoreCounter += 0.5;
		addFrame(60);
	}
}

void Puyo::Turn()
{
	SceneIngame* Scene = SceneIngame::getInstance();
	//TODO TurnCount == 1 —Ž‚¿‚½‚Æ‚« Á‚¦‚é
	
		if (Scene->map[pos._y + 1][pos._x] != nullptr)
		{
			if (Scene->map[pos._y + 1][pos._x + 1] == nullptr&&pos._x < 6)
			{
				delMap(pos._x, pos._y);
				pos._x++;
				pos._y++;
				map(pos._x, pos._y, STATE_FALL);
				return;
			}
		}

		if (Scene->map[pos._y][pos._x - 1] != nullptr)
		{
			if (Scene->map[pos._y + 1][pos._x - 1] == nullptr)
			{
				delMap(pos._x, pos._y);
				pos._x--;
				pos._y++;
				map(pos._x, pos._y, STATE_FALL);
				return;
			}
		}

		if (Scene->map[pos._y - 1][pos._x] != nullptr)
		{
			if (Scene->map[pos._y - 1][pos._x - 1] == nullptr&&pos._x > 1)
			{
				delMap(pos._x, pos._y);
				pos._x--;
				pos._y--;
				map(pos._x, pos._y, STATE_FALL);
				return;
			}
		}

		if (Scene->map[pos._y][pos._x + 1] != nullptr)
		{
			if (Scene->map[pos._y - 1][pos._x + 1] == nullptr)
			{
				delMap(pos._x, pos._y);
				pos._x++;
				pos._y--;
				map(pos._x, pos._y, STATE_FALL);
				return;
			}
		}
		
	
}

void Puyo::FreeFall()
{

	if (getFrame() >= 30)
	{
		if (!UnderCollision())
		{
			delMap(pos._x, pos._y);
			pos._y--;
			setFrame(0);
			map(pos._x, pos._y, STATE_FALL);
		}

	}

	addFrame(5);
}

void Puyo::Set()
{
	return;
}

bool Puyo::UnderCollision()
{
	SceneIngame* Scene = SceneIngame::getInstance();
	if (pos._y == 0)
	{
		_state = STATE_SET;
		map(pos._x, pos._y, STATE_SET);
		setLifeTime(0);
		return true;
	}
	if ((Scene->map[pos._y - 1][pos._x] != nullptr) && (Scene->map[pos._y - 1][pos._x]->_state != STATE_FALL))
	{
		_state = STATE_SET;
		map(pos._x, pos._y, STATE_SET);
		setLifeTime(0);
		return true;
	}
	else if ((Scene->map[pos._y - 1][pos._x] != nullptr) && (Scene->map[pos._y - 1][pos._x]->_state == STATE_FALL))
	{
		if (pos._y > 1 && Scene->map[pos._y - 2][pos._x] == nullptr)
		{
			return true;
		}
		else if (pos._y ==1|| Scene->map[pos._y - 2][pos._x] != nullptr)
		{
			_state = STATE_SET;
			map(pos._x, pos._y, STATE_SET);
			setLifeTime(0);
			return true;
		}
		
	}
	
	return false;
}



void Puyo::ObjDisp()
{
	glPushMatrix();
	{
		glColor4f(colorStatus[0], colorStatus[1], colorStatus[2], colorStatus[3]);
		glTranslatef(-0.7 + (pos._x - 1)*0.15, -0.75 + pos._y*0.15, 1);
		glutSolidSphere(0.075, 180, 5);
	}
	glPopMatrix();
	glColor4f(1, 1, 1, 1);
}

void Puyo::ColorSetup(COLORPATTERN _colorNumber)
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
	case COLOR_PURPLE:
		colorStatus[0] = 1;
		colorStatus[1] = 0;
		colorStatus[2] = 1;
		colorStatus[3] = 1;
		break;
	case COLOR_WHITE:
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
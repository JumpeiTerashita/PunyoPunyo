#include "Puyo.h"
#include <time.h>
#include <stdlib.h>
#include "../glut.h"
#include "Scene_InGame.h"
#include "GameManager.h"
#include "../Engine/InputManager.h"
#include "../Liblary/BitController.h"

static const unsigned char Is_checked = 1 << IS_CHECKED;
static const unsigned char Is_rotate = 1 << IS_ROTATE;
static const unsigned char Is_falling = 1 << IS_FALLING;
static const unsigned char Is_Freefall = 1 << IS_FREEFALL;
static const unsigned char Will_Delete = 1 << WILL_DELETE;

Puyo::Puyo(int _x, int _y, int _colors)
{
	setLifeTime(1);
	Status = 0;
	pos.set(_x, _y);
	ColorNumber = (COLORPATTERN)(genrand_int32() % _colors);
	//ColorNumber = (COLORPATTERN)(rand() % _colors);
	ColorSetup(ColorNumber);
	setSequence(&Puyo::Fall);

}

Puyo::Puyo(int _x, int _y, COLORPATTERN _setColor)
{
	setLifeTime(1);
	Status = Status;
	BitTakeaway(&Status, Is_checked | Will_Delete);
	pos.set(_x, _y);
	ColorNumber = _setColor;
	ColorSetup(_setColor);
}

void Puyo::ObjUpdate()
{
	runSequence();
	setPosYPriority(pos._y);
	setPosXPriority(pos._x);
}



void Puyo::delMap(int _x, int _y)
{
	SceneIngame::getInstance()->map[_y][_x] = nullptr;
}

void Puyo::map(int _x, int _y, COLORPATTERN _setColor)
{
	SceneIngame::getInstance()->map[_y][_x] = new Puyo(_x, _y, _setColor);
}


static int c = 0;

void Puyo::Fall()
{
	SceneIngame* Scene = SceneIngame::getInstance();

	//if (0 == c && Scene->cc == 1) {
	//	printf("------------------------------\n");
	//}
	//if (Scene->cc == 1) {
	//	printf("x = %d y = %d\n", pos._x, pos._y);
	//	/*std::string col[4] = { "ê‘","óŒ","ê¬", "â©" };
	//	printf("%s\n", col[ColorNumber].c_str());*/
	//	c++;
	//	c %= 2;
	//}
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

	if (BitChecker(Status, Is_Freefall)) setSequence(&Puyo::FreeFall);
	if (BitChecker(Scene->KeyFlag, KeyFlag_LEFT))
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

	}
	if (BitChecker(Scene->KeyFlag, KeyFlag_RIGHT))
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

	}


	if (BitChecker(Scene->KeyFlag, KeyFlag_Turn_CounterClockwise)) TurnCounterClockwise();
	if (BitChecker(Scene->KeyFlag, KeyFlag_Turn_Clockwise)) TurnClockwise();
}



void Puyo::TurnCounterClockwise()
{

	SceneIngame* Scene = SceneIngame::getInstance();

	if (BitChecker(Status, Is_rotate))
	{
		switch (Scene->ArrangeRelation)
		{
		case ARRANGERELATION_VERTICAL: //èc->âE
			if (!Search_There_is(pos._x + 1, pos._y + 1) && pos._x < 6)
			{
				delMap(pos._x, pos._y);
				pos._x++;
				pos._y++;
				map(pos._x, pos._y, ColorNumber);
				Scene->ArrangeRelation = ARRANGERELATION_RIGHT;
			}
			break;
		case ARRANGERELATION_RIGHT: //âE->ãtèc
			if (!Search_There_is(pos._x - 1, pos._y + 1) && pos._y < 14)
			{
				delMap(pos._x, pos._y);
				pos._x--;
				pos._y++;
				Scene->ArrangeRelation = ARRANGERELATION_COUNTERVERTICAL;
			}
			break;
		case ARRANGERELATION_COUNTERVERTICAL: //ãtèc->ç∂
			if (!Search_There_is(pos._x - 1, pos._y - 1) && pos._x > 1)
			{
				delMap(pos._x, pos._y);
				pos._x--;
				pos._y--;
				map(pos._x, pos._y, ColorNumber);
				Scene->ArrangeRelation = ARRANGERELATION_LEFT;
			}
			break;
		case ARRANGERELATION_LEFT: //ç∂->èc
			if (!Search_There_is(pos._x + 1, pos._y - 1) && pos._y > 1)
			{
				delMap(pos._x, pos._y);
				pos._x++;
				pos._y--;
				Scene->ArrangeRelation = ARRANGERELATION_VERTICAL;
			}
			break;
		}
		Scene->KeyFlag = 0;
	}
}

void Puyo::TurnClockwise()
{
	SceneIngame* Scene = SceneIngame::getInstance();
	if (BitChecker(Status, Is_rotate))
	{
		switch (Scene->ArrangeRelation)
		{
		case ARRANGERELATION_VERTICAL: //èc->ç∂
			if (!Search_There_is(pos._x - 1, pos._y + 1) && pos._x > 1)
			{
				delMap(pos._x, pos._y);
				pos._x--;
				pos._y++;
				map(pos._x, pos._y, ColorNumber);
				Scene->ArrangeRelation = ARRANGERELATION_LEFT;
			}
			break;
		case ARRANGERELATION_LEFT: //ç∂->ãtèc
			if (!Search_There_is(pos._x + 1, pos._y + 1) && pos._y < 14)
			{
				delMap(pos._x, pos._y);
				pos._x++;
				pos._y++;
				Scene->ArrangeRelation = ARRANGERELATION_COUNTERVERTICAL;
			}
			break;
		case ARRANGERELATION_COUNTERVERTICAL: //ãtèc->âE
			if (!Search_There_is(pos._x + 1, pos._y - 1) && pos._x < 6)
			{
				delMap(pos._x, pos._y);
				pos._x++;
				pos._y--;
				map(pos._x, pos._y, ColorNumber);
				Scene->ArrangeRelation = ARRANGERELATION_RIGHT;
			}
			break;
		case ARRANGERELATION_RIGHT: //âE->èc
			if (!Search_There_is(pos._x - 1, pos._y - 1) && pos._y > 1)
			{
				delMap(pos._x, pos._y);
				pos._x--;
				pos._y--;
				Scene->ArrangeRelation = ARRANGERELATION_VERTICAL;
			}
			break;
		}

		Scene->KeyFlag = 0;
	}
}

void Puyo::FreeFall()
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


bool Puyo::UnderCollision()
{
	SceneIngame* Scene = SceneIngame::getInstance();
	if (pos._y == 1)
	{
		BitTakeaway(&Status, Is_falling | Is_Freefall);
		map(pos._x, pos._y, ColorNumber);
		setLifeTime(0);
		return true;
	}

	if (Search_There_is(pos._x, pos._y - 1))
	{
		BitTakeaway(&Status, Is_falling | Is_Freefall);
		map(pos._x, pos._y, ColorNumber);
		setLifeTime(0);
		return true;
	}
	else if (Search_is_Falling(pos._x, pos._y - 1))
	{
		if (pos._y == 3 || Search_There_is(pos._x, pos._y - 2))
		{
			BitTakeaway(&Status, Is_falling | Is_Freefall);
			map(pos._x, pos._y, ColorNumber);
			setLifeTime(0);
			return true;
		}
	}
	return false;
}

bool Puyo::Search_is_Falling(int _x, int _y)
{
	if (SceneIngame::getInstance()->map[_y][_x] == nullptr) return false;
	if (BitChecker(SceneIngame::getInstance()->map[_y][_x]->Status, Is_falling)) return true;
	return false;
}

bool Puyo::Search_There_is(int _x, int _y)
{
	if (SceneIngame::getInstance()->map[_y][_x] == nullptr) return false;
	return true;
}


void Puyo::ObjDisp()
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
	default:
		break;
	}
	return;
}
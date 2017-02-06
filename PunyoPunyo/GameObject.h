#pragma once

#include "Vec2.h"

enum COLORPATTERN
{
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_PURPLE,
	COLOR_WHITE,
	COLOR_MAX
};

enum STATE
{
	STATE_FALL,
	STATE_FREEFALL,
	STATE_TOUCHDOWN,
	STATE_AFTERDELETE,
	STATE_SET
};

class GameObject
{
public:
	vec2 pos;
	
	COLORPATTERN ColorNumber;
	STATE _state;
	GameObject() {
		LifeTime = -1;
	};
	virtual ~GameObject() {};
	
	virtual void ObjUpdate() = 0;
	virtual void ObjDisp() = 0;

	inline void setLifeTime(const int time) { LifeTime = time; }

	inline int getLifeTime() const { return LifeTime; }

private:
	int LifeTime;
};

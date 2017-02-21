#pragma once

#include "Vec2.h"



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
private:
	int LifeTime;
	int LogicPriority;
	int RenderPriority;

public:
	vec2 pos;
	
	STATE _state;

	GameObject() {
		LifeTime = -1;
		LogicPriority = 0;
		RenderPriority = 0;
	};
	virtual ~GameObject() {};
	
	virtual void ObjUpdate() = 0;
	virtual void ObjDisp() = 0;

	//setter
	inline void setLifeTime(const int _time) { LifeTime = _time; }
	inline void setLogicPriority(const int _priority) { LogicPriority = _priority; }
	inline void setRenderPriority(const int _priority) { RenderPriority = _priority; }

	//getter
	inline int getLifeTime() const { return LifeTime; }
	inline int getLogicPriority() const { return LogicPriority; }
	inline int getRenderPriority() const { return RenderPriority; }


};

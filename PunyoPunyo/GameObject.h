#pragma once

#include "Vec2.h"

class GameObject
{
private:
	int LifeTime;
	int LogicPriority;
	int RenderPriority;

public:
	vec2 pos;
	

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

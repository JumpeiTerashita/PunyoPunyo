#pragma once

#include "../Liblary/Vec2.h"

class GameObject
{
private:
	int LifeTime;
	int PosXPriority;
	int PosYPriority;
	int RenderPriority;

public:
	vec2 pos;


	GameObject() {
		pos.set(0, 0);
		LifeTime = -1;
		PosXPriority = 0;
		PosYPriority = 0;
		RenderPriority = 0;
	};
	virtual ~GameObject() {};

	virtual void ObjUpdate() = 0;
	virtual void ObjDisp() = 0;

	//setter
	inline void setLifeTime(const int _time) { LifeTime = _time; }
	inline void setPosXPriority(const int _priority) { PosXPriority = _priority; }
	inline void setPosYPriority(const int _priority) { PosYPriority = _priority; }
	inline void setRenderPriority(const int _priority) { RenderPriority = _priority; }

	//getter
	inline int getLifeTime() const { return LifeTime; }
	inline int getPosXPriority() const { return PosXPriority; }
	inline int getPosYPriority() const { return PosYPriority; }
	inline int getRenderPriority() const { return RenderPriority; }


};

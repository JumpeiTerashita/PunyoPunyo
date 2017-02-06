#pragma once

#include "GameObject.h"
#include "BufferController.h"

class Collision
{
public:
	Collision() {};
	~Collision() {};
	static bool CollisionDetection(Buffer* BUFFER, GameObject* OBJ);
	static int underCollision(Buffer* BUFFER, int _x,int _y);
private:

};




#include "Collision.h"
#include "BufferController.h"

bool Collision::CollisionDetection(Buffer* BUFFER, GameObject* OBJ)
{
	vec2 nowPos = OBJ->pos;
	
	const int lookPos_y = (nowPos._y + 13 - 1) % 13;
	
	
	if (BUFFER->layout[lookPos_y][nowPos._x])
	{
		return true;
	}

	if (nowPos._y == 0)
	{
		return true;
	}

	
	return false;
}

int Collision::underCollision(Buffer* BUFFER, int _x, int _y)
{	
	if (_y==0)
	{
		if (!BUFFER->layout[0][_x]) 
		{ 
			return 0; 
		}
		else return 1;
	}
	if (BUFFER->layout[_y][_x])
	{
		return _y+1;
	}
	Collision::underCollision(BUFFER, _x, ((_y + 13 - 1) % 13));
}

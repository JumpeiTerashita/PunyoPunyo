#include <stdlib.h>
#include "glut.h"
#include "BufferController.h"
#include "GameObject.h"
#include "Collision.h"

void Buffer::layoutMap(GameObject* PUYO)
{
	/*layout[PUYO->pos._y][PUYO->pos._x] = 1;
	layout[PUYO->subPos._y][PUYO->subPos._x] = 1;*/
	int look_x = PUYO->pos._x;
	int look_y = (PUYO->pos._y + 13 - 1) % 13;
	int underPuyoPos = Collision::underCollision(this, look_x, look_y);
	layout[underPuyoPos][look_x] = PUYO->ColorNumber;
	
	return;
}

void Buffer::ObjUpdate()
{

}

void Buffer::ObjDisp()
{
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (layout[i][j] != 0)
			{
				glPushMatrix();
				{
					int color = layout[i][j];
					
					glTranslatef(-0.7 + j*0.15, -0.75 + i*0.15, 1);
					glutSolidSphere(0.075, 180, 5);
				}
				glPopMatrix();
			}
		}
	}
}

//bool Buffer::ConnectSearch(int _x, int _y, int Cnt)
//{
//	static int ConnectSearch = 0;
//
//	if (_x - 1 > 0 && (layout[_y][_x - 1] == layout[_y][_x])) ConnectSearch++;  
//	if (_x + 1 < 6 && (layout[_y][_x + 1] == layout[_y][_x])) ConnectSearch++;
//	if (_y - 1 > 0 && (layout[_y-1][_x] == layout[_y][_x])) ConnectSearch++;
//	if (_y - 1 > 0 && (layout[_y][_x - 1] == layout[_y][_x])) ConnectSearch++;
//
//}
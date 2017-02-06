#pragma once
#include "GameObject.h"



class Buffer:public GameObject
{
public:
	Buffer()
	{
		layout[0][0] = 1;
		layout[0][1] = 2;
		layout[1][0] = 1;
		layout[1][1] = 1;
	}
	~Buffer() {};

	void layoutMap(GameObject* PUYO);

	void ObjUpdate();
	void ObjDisp();

	int layout[13][6] = { 0 };
	bool Checked[13][6] = { 0 };

	
private:

};


#pragma once

#include <list>
#include "Puyo.h"
#include "../Engine/GameObject.h"

enum DIFFICULTY
{
	EASY,
	NORMAL,
	HARD,
	EXTREME,
	LUNATIC,
	DIFFICULTY_NUM
};

class GameManager :public Sequence<GameManager>
{
public:
	GameManager();
	~GameManager() {};
	static GameManager* getInstance();

	inline void addObject(GameObject* object) {
		objects.push_back(object);
	}

	unsigned char* pixels = (unsigned char*)malloc(300 * 300 * 3);
	FILE *pFile;

	void Scene_ReadError();
	void Scene_Title();
	void Scene_DifficultySelect();
	void Scene_Ingame();

	bool MoveSearch(int lookX);

	void update();
	void display();

	int HighScore;
	int SelectDifficulty;

	void glColorSetup(int _colorNumber);

	inline std::list< GameObject* > getObject()
	{
		return objects;
	}

	inline std::list< GameObject* >::iterator getObjectsIterator() 
	{
		std::list< GameObject* >::iterator it = GameManager::getInstance()->objects.begin();
		return it;
	}

	static bool sortPosXPriority_Descending(const GameObject* objA, const GameObject* objB)
	{
		return objA->getPosXPriority() > objB->getPosXPriority(); //大きい順ソート
	}

	static bool sortPosXPriority_Ascending(const GameObject* objA, const GameObject* objB)
	{
		return objA->getPosXPriority() < objB->getPosXPriority(); //小さい順ソート
	}

private:
	static GameManager* instance;
	std::list< GameObject* > objects;
	
	static bool sortPosYPriority(const GameObject* objA, const GameObject* objB) 
	{
		return objA->getPosYPriority() < objB->getPosYPriority(); //小さい順ソート
	}

	static bool sortRenderPriority(const GameObject* objA, const GameObject* objB) 
	{
		return objA->getRenderPriority() < objB->getRenderPriority(); //小さい順ソート
	}

	const char DifficultyDisp[DIFFICULTY_NUM][10] = { "EASY","NORMAL","HARD","EXTREME","LUNATIC" };
	
	
	
};







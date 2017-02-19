#pragma once

#include <list>
#include "Puyo.h"
#include "GameObject.h"

class GameManager :public Sequence<GameManager>
{
public:
	GameManager();
	~GameManager() {};
	static GameManager* getInstance();

	inline void addObject(GameObject* object) {
		objects.push_back(object);
	}

	void Scene_Title();
	void Scene_Ingame();

	bool MoveSearch(int lookX);

	void update();
	void display();

	int HighScore;
	
	std::list< GameObject* > getObjects();

	inline int GetObjectNum() {
		return objects.size();
	}

private:
	static GameManager* instance;
	std::list< GameObject* > objects;
	
	static bool sortLogicPriority(const GameObject* objA, const GameObject* objB) {
		return objA->getLogicPriority() < objB->getLogicPriority(); //小さい順ソート
	}

	static bool sortRenderPriority(const GameObject* objA, const GameObject* objB) {
		return objA->getRenderPriority() < objB->getRenderPriority(); //小さい順ソート
	}
};

//TODO class SceneIngame を　別ファイルに引っ越し





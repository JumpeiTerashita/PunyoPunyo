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


	void update();
	void display();

	int HighScore;
	

private:
	static GameManager* instance;
	std::list< GameObject* > objects;

	
};

//TODO class SceneIngame を　別ファイルに引っ越し





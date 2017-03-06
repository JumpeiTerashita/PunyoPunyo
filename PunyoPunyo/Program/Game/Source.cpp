#pragma comment(linker,"/SUBSYSTEM:\"WINDOWS\" /ENTRY:\"mainCRTStartup\"") //コンソールウィンドウ非表示

#include "../Engine/InputManager.h"
#include "GameManager.h"
#include "../MT.h"
#include "../glut.h"
#include <time.h>

void display();
void update(int);

int main(int argc, char* argv[])
{
	init_genrand((unsigned)time(NULL));

	glutInit(&argc, argv);

	glutInitWindowSize(640, 640);

	glutCreateWindow("PunyoPunyo");

	glutDisplayFunc(display);

	glutTimerFunc(0, update, 0);

	glutKeyboardFunc(Key);
	glutKeyboardUpFunc(UpKey);
	glutSpecialFunc(SpecialKey);
	
	glutMainLoop();

	return 0;
}




void display()
{
	GameManager::GetInstance()->display();

	glFlush();
}

void update(int value)
{
	//glutForceJoystickFunc();
	
	GameManager::GetInstance()->update();
	
	glutPostRedisplay();

	glutTimerFunc(1000/60, update, 0);
	
}

//TODO 変数名　一貫性を！！！
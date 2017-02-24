#pragma comment(linker,"/SUBSYSTEM:\"WINDOWS\" /ENTRY:\"mainCRTStartup\"") //�R���\�[���E�B���h�E��\��


#include "../Engine/InputManager.h"
#include "GameManager.h"
#include "../glut.h"
#include <time.h>


void display();
void update(int);

int main(int argc, char* argv[])
{
	

	srand((unsigned)time(NULL));

	glutInit(&argc, argv);

	glutInitWindowSize(640, 640);

	glutCreateWindow("Puyo!");

	

	glutDisplayFunc(display);

	glutTimerFunc(0, update, 0);

	//glutJoystickFunc(JoyStick, 0);

	glutKeyboardFunc(Key);
	glutKeyboardUpFunc(UpKey);
	glutSpecialFunc(SpecialKey);
	
	glutMainLoop();

	return 0;
}




void display()
{
	GameManager::getInstance()->display();

	glFlush();
}

void update(int value)
{
	//glutForceJoystickFunc();
	
	GameManager::getInstance()->update();
	
	glutPostRedisplay();

	glutTimerFunc(1000/60, update, 0);
	
}

//TODO �ϐ����@��ѐ����I�I�I
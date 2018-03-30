#pragma once
#include "core.h"
#include <GL/glut.h>
#include "magnitudes.h"
using namespace std;



class character
{
public:
	double	speed = 0.02; // Множитель скорости изменения координат (скорость передвижения)
	double	CurrentFrame = 0; // Текуший кадр анимации
	double	CurrentAnimation = 0;	//Текущая анимация 0 - стоим, 1 - идем
	Vector	Position, // Позиция
			Velocity; // Скорость
			/*ЕБАНОЕ УСКОРЕНИЕ*/
	//static void StaticAnimation(int value) {
	//	character *thePtr = reinterpret_cast<character*>(value);
	//	thePtr->Animation();
	//}
	void Animation();
	void Update();

private:
};

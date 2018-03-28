#pragma once
#include <cmath>
#include <GL/glut.h>
#include <iostream>
using namespace std;
struct Vector
{
	double X, Y, Len;
	Vector()
	{
		X = 0;
		Y = 0;
		Len = 0;
	}
	Vector(double x, double y)
	{
		X = x;
		Y = y;
		Len = GetLength();
	}
	double GetLength()
	{
		double len = sqrt(X * X + Y * Y);
		return len;
	}
	Vector GetNormalize()
	{
		double len = GetLength();
		if (len == 0)
			return Vector();
		double x = X / len;
		double y = Y / len;
		return Vector(x, y);
	}
};

class character
{
public:
	double	speed = 0.03; // Множитель скорости изменения координат (скорость передвижения)
	double	CurrentFrame = 0; // Текуший кадр анимации
	double	CurrentAnimation = 0;	//Текущая анимация 0 - стоим, 1 - идем
	Vector	Position, // Позиция
			Velocity; // Скорость
			/*ЕБАНОЕ УСКОРЕНИЕ*/

	static void StaticUpdateCB(int value) {
		character *thePtr = reinterpret_cast<character*>(value);
		thePtr->UpdateCB();
	}

	void UpdateCB() {
		cout << "a\n";
		glutTimerFunc(100, character::StaticUpdateCB, 1);
	}
private:
};

void Update(int value);
void Animation(int value);
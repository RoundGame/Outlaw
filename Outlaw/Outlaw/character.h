#pragma once
#include <cmath>
#include <GL/glut.h>
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
	double	speed; // Множитель скорости изменения координат (скорость передвижения)
	double	CurrentFrame; // Текуший кадр анимации
	double	CurrentAnimation;	//Текущая анимация 0 - стоим, 1 - идем
	Vector	Position, // Позиция
			Velocity; // Скорость
			/*ЕБАНОЕ УСКОРЕНИЕ*/
	static void StaticAnimation(int value) {
		character *thePtr = reinterpret_cast<character*>(value);
		thePtr->Animation();
	}

	void Animation();

private:
};

void Update();
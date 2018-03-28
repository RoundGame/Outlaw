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

	void Velocity_null()
	{
		Velocity.X = 0;
		Velocity.Y = 0;
		Velocity.Len = 0;
	}
	static void StaticUpdate(int value) {
		character *thePtr = reinterpret_cast<character*>(value);
		thePtr->Update();
	}

	void Update() {
		glutPostRedisplay(); //Обновляем экран
		Vector velocity = Velocity.GetNormalize(); //Нормализуем вектор скорости
		if (velocity.Len != 0) //Если есть скорость то
		{
			CurrentAnimation = 1; //Включаем анимацию передвижения
			Position.X += velocity.X * speed; //Добавляем к вектору игрока вектор его скорости
			Position.Y += velocity.Y * speed;
		}
		else //Иначе
		{
			CurrentFrame = 0; //Текущий кадр - нулевой (стоим)
			CurrentAnimation = 0; //Текущая анимация нулевая (стоим)
		}
		glutTimerFunc(200, character::StaticUpdate, 0); // Задержка 20 мс перед новым вызовом функции
	}
	//void Animation(int value) //Обновляем Кадры исходя из текущей анимации
	//{
	//	if (player.CurrentAnimation == 1)
	//	{
	//		player.CurrentFrame++;
	//		if (player.CurrentFrame > 4) //В анимации пять кадров, поэтому сбрасываем счетчик на 0, как только он перевалил за 4
	//			player.CurrentFrame = 0;
	//	}
	//	glutTimerFunc(100, Animation, 1); //Задержка 100 мс перед новым вызовом функции
	//}
private:
};
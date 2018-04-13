#pragma once
#define _USE_MATH_DEFINES
#include "key.h"
#include <cmath>

// Обращение к клавишам управления
enum gamekey { LEFT, RIGHT, UP, DOWN };

struct Vector
{
	double	X,
			Y;
	Vector()
	{
		X = 0;
		Y = 0;
	}
	Vector(double x, double y)
	{
		X = x;
		Y = y;
	}

	/*Получение длины вектора*/
	double GetLength()
	{
		double len = sqrt(X * X + Y * Y);
		return len;
	}
	/*Нормализация вектора*/
	Vector GetNormalize()
	{
		if (GetLength() == 0)
			return Vector();
		double x = X / GetLength();
		double y = Y / GetLength();
		return Vector(x, y);
	}
};

struct Key
{
	int Nominal;
	bool isPressed;
	Key()
	{
		Nominal = KEY_UNKNOWN;
		isPressed = false;
	}
};

struct Room
{
	unsigned int Texture;
	float Size;
};

struct Entity
{
	float Size;
	unsigned int Texture;
	bool Barrier; // Является преградой? 
	Vector Position;
	Entity()
	{
		Size = 0.1;
		Barrier = false;
	}
};
#pragma once
#include <cmath>
struct Vector
{
	double	X,
			Y,	
			Len; /*Длина*/
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

	/*Получение длины вектора*/
	double GetLength()
	{
		double len = sqrt(X * X + Y * Y);
		return len;
	}
	/*Нормализация вектора*/
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
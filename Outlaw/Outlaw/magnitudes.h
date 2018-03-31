#pragma once
#include <cmath>
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

	/*��������� ����� �������*/
	double GetLength()
	{
		double len = sqrt(X * X + Y * Y);
		return len;
	}
	/*������������ �������*/
	Vector GetNormalize()
	{
		if (GetLength() == 0)
			return Vector();
		double x = X / GetLength();
		double y = Y / GetLength();
		return Vector(x, y);
	}
};
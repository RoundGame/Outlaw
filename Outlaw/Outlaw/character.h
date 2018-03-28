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
	double	speed; // ��������� �������� ��������� ��������� (�������� ������������)
	double	CurrentFrame; // ������� ���� ��������
	double	CurrentAnimation;	//������� �������� 0 - �����, 1 - ����
	Vector	Position, // �������
			Velocity; // ��������
			/*������ ���������*/
	static void StaticAnimation(int value) {
		character *thePtr = reinterpret_cast<character*>(value);
		thePtr->Animation();
	}

	void Animation();

private:
};

void Update();
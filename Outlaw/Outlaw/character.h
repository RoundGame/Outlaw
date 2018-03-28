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
	double	speed = 0.03; // ��������� �������� ��������� ��������� (�������� ������������)
	double	CurrentFrame = 0; // ������� ���� ��������
	double	CurrentAnimation = 0;	//������� �������� 0 - �����, 1 - ����
	Vector	Position, // �������
			Velocity; // ��������
			/*������ ���������*/

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
		glutPostRedisplay(); //��������� �����
		Vector velocity = Velocity.GetNormalize(); //����������� ������ ��������
		if (velocity.Len != 0) //���� ���� �������� ��
		{
			CurrentAnimation = 1; //�������� �������� ������������
			Position.X += velocity.X * speed; //��������� � ������� ������ ������ ��� ��������
			Position.Y += velocity.Y * speed;
		}
		else //�����
		{
			CurrentFrame = 0; //������� ���� - ������� (�����)
			CurrentAnimation = 0; //������� �������� ������� (�����)
		}
		glutTimerFunc(200, character::StaticUpdate, 0); // �������� 20 �� ����� ����� ������� �������
	}
	//void Animation(int value) //��������� ����� ������ �� ������� ��������
	//{
	//	if (player.CurrentAnimation == 1)
	//	{
	//		player.CurrentFrame++;
	//		if (player.CurrentFrame > 4) //� �������� ���� ������, ������� ���������� ������� �� 0, ��� ������ �� ��������� �� 4
	//			player.CurrentFrame = 0;
	//	}
	//	glutTimerFunc(100, Animation, 1); //�������� 100 �� ����� ����� ������� �������
	//}
private:
};
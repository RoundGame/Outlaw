#pragma once
#include <GL/glut.h>
#include "magnitudes.h"

struct Character
{
	double  Direction = 3; // �������� ����������� ��������� (3 - ������� ������)
	double	speed = 0.4; // ��������� �������� ��������� ��������� (�������� ������������)
	double	boost = 4; //����������� ���������. ���� �� �������, ����������� ���������� � ��������������� (������� �����������), ���� ���������, �� �������� ���������� � �������� ��������������� (�������� ��)
	double	CurrentFrame = 0; // ������� ���� ��������
	bool	CurrentAnimation = 0;	//������� �������� 0 - �����, 1 - ����
	unsigned int Texture; // �������� ���������
	Vector	Position, // �������
			Velocity, // ��������
			Acceleration; // ���������
	
	void Animation(int frame); // �������� ���������
	void Update();	  // ������� ��������� ����������� ���������
	void Draw();	// ��������� ��������� �� ����� ����������� � ����.
};
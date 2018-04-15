#pragma once
#include <GL/glut.h>
#include "magnitudes.h"

struct Physical_component
{
	double	Speed = 0.4; // ��������� �������� ��������� ��������� (�������� ������������)
	double	Boost = 4; //����������� ���������. ���� �� �������, ����������� ���������� � ��������������� (������� �����������), ���� ���������, �� �������� ���������� � �������� ��������������� (�������� ��)
	double Angle = 0; //���� �������� ������������ ������� � ������������ (0, 1)
	Vector	Position, // �������
			Velocity, // ��������
			Acceleration; // ���������

	void Update(bool isAcceleration);
};

struct Object
{
	bool isExist = false;
	Sprite Body;
	Physical_component Physics;
};

struct Character
{
	/*��������� ����������������*/ double	Direction = 3;	// �������� ����������� ��������� (3 - ������� ������) 
	/*��������� ����������������*/ double	CurrentFrame = 0;	// ������� ���� ��������
	/*��������� ����������������*/ bool		CurrentAnimation = 0;	// ������� �������� 0 - �����, 1 - ����
	
	Physical_component Physics; // ���������� ��������� ��������� (������, ��������, ���������)
	Sprite Leg, Body;	// �������� ���������

	void Animation();	// �������� ���������
	void Update();		// ������� ��������� ����������� ���������
	void Draw();		// ��������� ��������� �� ����� ����������� � ����.
};
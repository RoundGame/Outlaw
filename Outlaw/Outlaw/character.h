#pragma once
#include <GL/glut.h>
#include "magnitudes.h"


struct Physical_component
{
	double	Speed = 0.3; // ��������� �������� ��������� ��������� (�������� ������������)
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

struct Static_Object
{
	bool isExist = false;
	Sprite Body;
	Vector Position;
};

struct Character
{
	/*��������� ����������������*/ double	Direction = 0;	// �������� ����������� ��������� (3 - ������� ������) 
	/*��������� ����������������*/ double	CurrentFrame = 0;	// ������� ���� ��������
	
	Physical_component Physics; // ���������� ��������� ��������� (������, ��������, ���������)
	Sprite Legs, Body;	// �������� ���������

	void Animation();	// �������� ���������
	void Target_TO(Vector Position); // ���������� ����(Physics.Angle) ����������� � ������� ������� ����
	void Draw();		// ��������� ��������� �� ����� ����������� � ����.
};
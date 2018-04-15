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
	unsigned int Texture;
	Physical_component Physics;
};

struct Character
{
	/*��������� ����������������*/ double  Direction = 3; // �������� ����������� ��������� (3 - ������� ������) 
	/*��������� ����������������*/ double	CurrentFrame = 0; // ������� ���� ��������
	/*��������� ����������������*/ bool	CurrentAnimation = 0;	//������� �������� 0 - �����, 1 - ����
	unsigned int Texture1, Texture2; // �������� ���������
	/*�������� ���� ������� ��������*/ int texture_frame = 8; // ���-�� ������ ��������
	Physical_component Physics;
	void Animation(int frame); // �������� ���������
	void Update();	  // ������� ��������� ����������� ���������
	void Draw();	// ��������� ��������� �� ����� ����������� � ����.
};
#pragma once
#include <GL/glut.h>
#include "magnitudes.h"

struct Object 
{
	bool isExist = false;
	unsigned int Texture;
	double Angle = 0; //���� �������� ������������ ������� � ������������ (0, 1)
	Vector	Position, // �������
			Velocity, // ��������
			Acceleration; // ���������
};

struct Character
{
	/*��������� ����������������*/ double  Direction = 3; // �������� ����������� ��������� (3 - ������� ������) 
	double	Speed = 0.4; // ��������� �������� ��������� ��������� (�������� ������������)
	double	Boost = 4; //����������� ���������. ���� �� �������, ����������� ���������� � ��������������� (������� �����������), ���� ���������, �� �������� ���������� � �������� ��������������� (�������� ��)
	/*��������� ����������������*/ double	CurrentFrame = 0; // ������� ���� ��������
	/*��������� ����������������*/ bool	CurrentAnimation = 0;	//������� �������� 0 - �����, 1 - ����
	unsigned int Texture1, Texture2; // �������� ���������
	/*�������� ���� ������� ��������*/ int texture_frame = 8; // ���-�� ������ ��������
	Object Move;
	void Animation(int frame); // �������� ���������
	void Update();	  // ������� ��������� ����������� ���������
	void Draw();	// ��������� ��������� �� ����� ����������� � ����.
};
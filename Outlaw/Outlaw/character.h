#pragma once
#include "magnitudes.h"

struct Character
{
	double  Direction = 3;
	double	speed = 0.02; // ��������� �������� ��������� ��������� (�������� ������������)
	double	boost = 4; //����������� ���������. ���� �� �������, ����������� ���������� � ��������������� (������� �����������), ���� ���������, �� �������� ���������� � �������� ��������������� (�������� ��)
	double	CurrentFrame = 0; // ������� ���� ��������
	bool	CurrentAnimation = 0;	//������� �������� 0 - �����, 1 - ����
	Vector	Position, // �������
			Velocity, // ��������
			Acceleration; // ���������
	void Animation(int frame); // �������� ���������
	void Update();	  // ������� ��������� ����������� ���������
};
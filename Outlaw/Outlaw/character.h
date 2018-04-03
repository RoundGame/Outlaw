#pragma once
#include "magnitudes.h"

struct Character
{
	double	speed = 0.02; // ��������� �������� ��������� ��������� (�������� ������������)
	double	CurrentFrame = 0; // ������� ���� ��������
	bool	CurrentAnimation = 0;	//������� �������� 0 - �����, 1 - ����
	Vector	Position, // �������
			Velocity, // ��������
			Acceleration; // ���������
	void Animation(int frame); // �������� ���������
	void Update();	  // ������� ��������� ����������� ���������
};

#pragma once
#include <GL/glut.h>
#include "magnitudes.h"

struct character
{
	double	speed = 0.02; // ��������� �������� ��������� ��������� (�������� ������������)
	double	CurrentFrame = 0; // ������� ���� ��������
	bool	CurrentAnimation = 0;	//������� �������� 0 - �����, 1 - ����
	Vector	Position, // �������
			Velocity; // ��������
					  /* ���������*/
	void Animation(int frame); // �������� ���������
	void Update();	  // ������� ��������� ����������� ���������
};

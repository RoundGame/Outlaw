#pragma once
#include "core.h"
#include <GL/glut.h>
#include "magnitudes.h"
using namespace std;



class character
{
public:
	double	speed = 0.02; // ��������� �������� ��������� ��������� (�������� ������������)
	double	CurrentFrame = 0; // ������� ���� ��������
	double	CurrentAnimation = 0;	//������� �������� 0 - �����, 1 - ����
	Vector	Position, // �������
			Velocity; // ��������
			/*������ ���������*/
	//static void StaticAnimation(int value) {
	//	character *thePtr = reinterpret_cast<character*>(value);
	//	thePtr->Animation();
	//}
	void Animation();
	void Update();

private:
};

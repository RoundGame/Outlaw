#include "character.h"

void character::Animation() 
{
	if (CurrentAnimation == 1)
	{
		CurrentFrame++;
		if (CurrentFrame > 4) //� �������� ���� ������, ������� ���������� ������� �� 0, ��� ������ �� ��������� �� 4
			CurrentFrame = 0;
	}
	glutTimerFunc(100, character::StaticAnimation, 1); //�������� 100 �� ����� ����� ������� �������
};
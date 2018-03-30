#include "character.h"

void character::Animation() 
{
	if (CurrentAnimation == 1)
	{
		CurrentFrame++;
		if (CurrentFrame > 4) //� �������� ���� ������, ������� ���������� ������� �� 0, ��� ������ �� ��������� �� 4
			CurrentFrame = 0;
	}
}

void character::Update() 
{
	Vector velocity = Velocity.GetNormalize(); //����������� ������ ��������
	if (velocity.Len != 0) //���� ���� �������� ��
	{
		CurrentAnimation = 1; //�������� �������� ������������
		Position.X += velocity.X * speed; //��������� � ������� ������ ������ ��� ��������
		Position.Y += velocity.Y * speed;
	}
	else //�����
	{
		CurrentFrame = 0; //������� ���� - ������� (�����)
		CurrentAnimation = 0; //������� �������� ������� (�����)
	}
}


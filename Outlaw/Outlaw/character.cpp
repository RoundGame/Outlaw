#include "character.h"

void character::Animation(int frame)
{
	if (CurrentAnimation)
	{
		CurrentFrame++;
		if (CurrentFrame > frame) //� �������� ���� ������, ������� ���������� ������� �� 0, ��� ������ �� ��������� �� 4
			CurrentFrame = 0;
	}
}

void character::Update() 
{
	Vector velocity = Velocity.GetNormalize(); //����������� ������ ��������
	if (velocity.GetLength() != 0) //���� ���� �������� ��
	{
		CurrentAnimation = true; //�������� �������� ������������
		Position.X += velocity.X * speed; //��������� � ������� ������ ������ ��� ��������
		Position.Y += velocity.Y * speed;
	}
	else //�����
	{
		CurrentFrame = 0; //������� ���� - ������� (�����)
		CurrentAnimation = false; //������� �������� ������� (�����)
	}
}


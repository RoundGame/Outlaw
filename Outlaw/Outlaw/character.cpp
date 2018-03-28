#include "character.h"

void character::Update() {
	glutPostRedisplay(); //��������� �����
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
	glutTimerFunc(200, character::StaticUpdate, 0); // �������� 20 �� ����� ����� ������� �������
}
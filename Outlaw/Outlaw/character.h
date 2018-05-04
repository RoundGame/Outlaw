#pragma once
#include <GL/glut.h>
#include "magnitudes.h"


struct Physical_component
{
	double	Speed = 0.2; // ��������� �������� ��������� ��������� (�������� ������������)
	double	Boost = 5; //����������� ���������. ���� �� �������, ����������� ���������� � ��������������� (������� �����������), ���� ���������, �� �������� ���������� � �������� ��������������� (�������� ��)
	double Angle = 0; //���� �������� ������������ ������� � ������������ (0, 1)
	Vector	Position, // �������
			Velocity, // ��������
			Acceleration; // ���������

	void Update(bool isAcceleration);
};

struct Object
{
	bool isExist = false;
	Sprite Body;
	Physical_component Physics;
};

struct Static_Object
{
	bool isExist = false;
	Sprite Body;
	Vector Position;
};

struct Character
{
	bool isAttack = false;
	bool isKick = false;
	double HP = 100;
	double Knock_Back = 0.6;
	/*��������� ����������������*/ double	Direction = 0;	// �������� ����������� ��������� (3 - ������� ������) 
	/*��������� ����������������*/ double	Legs_CurrentFrame = 0;	// ������� ���� ��������
	double	Attack_CurrentFrame = 0;
	
	Physical_component Physics; // ���������� ��������� ��������� (������, ��������, ���������)
	Sprite Legs, Body, Death, Attack;	// �������� ���������

	void Animation();	// �������� ���������
	void Target_To(Vector Target, Vector Window_Render_Size); // ���������� ����(Physics.Angle) ����������� � ������� ������� ����
	void Set_Legs_Direction();
	void Draw();		// ��������� ��������� �� ����� ����������� � ����.
	void Use_Collisions(Static_Object Obj[], int obj_count);
};
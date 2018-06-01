#pragma once
#include <ctime>
#include <list>
#include "config.h"
#include <windows.h>
#include "core.h"
using namespace std;
/*	��� �������
EMPTY	- ������,
NORMAL	- �������(�� �������),
SHOP	- �������,
BOSS	- ����� �����,
SECRET	- ������	*/
enum RoomType
{
	EMPTY = 0, NORMAL, SHOP, BOSS, SECRET
};

struct level
{
	struct room
	{
		room
			*up,
			*down,
			*right,
			*left;
		RoomType type;
		char box[room_h][room_w];
		room(RoomType _T); // ������� ������� ���� _T
		unsigned __int8 enemy_life = 0; // ���-�� ����� 
		unsigned __int8 enemy_die = 0; // ���-�� ������� �����������
	};
	struct pos // ����� �� �����
	{
		__int8 h; // ������
		__int8 w; // ������
		room *link = nullptr; // ��������� �� ������� � ���� �����, �� ������� ������� ���
		
		pos(__int8 _h, __int8 _w, room * _link); // ������� ����� �� ����� � ������� _h(������) _w(������) _link(��������� �� �������)
	};
	
	room * center = new room(EMPTY); // ����������� �������
	room * current = center;
	list<pos> nextgen; // ������� ����� � ������� �������� ���������� �����
	
	unsigned __int8 neighbors = 0; // ����� �������
	unsigned __int8 rooms = 0; // ����� �������
	unsigned __int8 error = 0; // ���������� ����������������� ������ ������, ���������� ��� ���������� ��������� �������
	unsigned char box[level_size][level_size]; // ����� [������][������]

	pos getnextpos(); // ��������� ��������� �� ������� ��������� �����

	void draw(); // ����� ����� � �������
	void draw(room * room); // ������ ������� � �������
	bool create(__int8 h, __int8 w); /* � ������ � ������������� ������ ��������� ����� (h,w) ��� ������� �������,
										����������� ����� ������� ��� �� �������� � ���������� true, ����� false*/
	void generation(); /*���� ������ ����� ��������� � ���������� ����� �� ������, ��
					   �������� ������ ��������� �� ����������, � ��������� � ���������
					   ������������ ������� �����������(����� �������) � ���������� �� 
					   � ������ ����� ��������� � ���������� �����*/
	void build(room * room); // ���������� ���������� �������

	level(); // ����������� �������� ������ ����� � ��������� �������� � ������
};
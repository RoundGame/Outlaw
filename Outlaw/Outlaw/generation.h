#pragma once
#include <ctime>
#include <list>
#include "config.h"

#include <windows.h>
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
	const char room_d = '#'; // ������ ������� �� ����� ������������ �������
	const char empty_d = ' '; // ������ ������� �� ����� ������������ �������

	struct room
	{
		room
			*up = nullptr,
			*down = nullptr,
			*right = nullptr,
			*left = nullptr;
		RoomType type = NORMAL;

		room(RoomType _T); //������� ������� ���� _T
	};
	struct pos // ����� �� �����
	{
		__int8 h; // ������
		__int8 w; // ������
		room *link = nullptr; // ��������� �� ������� � ���� �����, �� ������� ������� ���
		
		pos(__int8 _h, __int8 _w, room * _link); // ������� ����� �� ����� � ������� _h(������) _w(������) _link(��������� �� �������)
	};
	
	room * center = new room(EMPTY); // ����������� �������

	char box[level_size][level_size]; // ����� [������][������]
	
	list<pos> nextgen; // ������� ����� � ������� �������� ���������� �����
	
	__int8 neighbors = 0; // ����� �������
	__int8 error = 0; // ���������� ����������������� ������ ������, ���������� ��� ���������� ��������� �������

	pos getnextpos(); // ��������� ��������� �� ������� ��������� �����


	void draw(); // ����� ����� �� �����

	bool create(__int8 h, __int8 w); /* � ������ � ������������� ������ ��������� ����� (h,w) ��� ������� �������,
										����������� ����� ������� ��� �� �������� � ���������� true, ����� false*/

	void generation(); /*���� ������ ����� ��������� � ���������� ����� �� ������, ��
					   �������� ������ ��������� �� ����������, � ��������� � ���������
					   ������������ ������� �����������(����� �������) � ���������� �� 
					   � ������ ����� ��������� � ���������� �����*/

	level(); // ����������� �������� ������ ����� � ��������� �������� � ������
};
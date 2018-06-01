#include "generation.h"

level::room::room(RoomType _T)
{
	srand(time(0) * rand());
	type = _T;
	up = down = right = left = nullptr;

	////////////////// ������ ������� ������ /////////////////////////////////
	for (unsigned __int8 i = 1; i < room_h - 1; i++) // ������� ���
	{
		for (unsigned __int8 j = 1; j < room_w - 1; j++)
		{
			box[i][j] = room_floor;
		}
	}

	int k = 0;
	for (unsigned __int8 i = 1 + safe_indent; i < room_h - 1 - safe_indent; i++) // ���������� ��������� �����������
	{
		for (unsigned __int8 j = 1 + safe_indent; j < room_w - 1 - safe_indent; j++)
		{
			if (rand() % wall_frequency == 0)
			{
				box[i][j] = room_wall;
			}
		}
	}

	for (unsigned __int8 i = 0; i < enemy_size; i++) // ���������� ��������� ������
	{
		box[rand() % room_h][rand() % room_w] = room_enemy;
	}
	if (rand() % prob_gift == 0)
	{
		box[rand() % (room_h-1) + 1][rand() % (room_w - 1) + 1 ] = room_gift; // ������� � �������
	}

	for (int i = 0; i < room_h; i++) // ������ ����� ( ����� � ������)
	{
		box[i][0] = room_wall;
		box[i][room_w - 1] = room_wall;
	}

	for (int j = 1; j < room_w - 1; j++) // ������ ����� ( ������� � ������)
	{
		box[0][j] = room_wall;
		box[room_h - 1][j] = room_wall;
	}
	//////////////////////////////////////////////////////////////////////////
}

void level::build(room * room) // ���������� ���������� �������
{
	if (room->up)
	{
		room->box[0][room_w / 2] = room_door;
		room->box[0][(room_w / 2) - 1] = room_door;
	}
	if (room->down)
	{
		room->box[room_h - 1][room_w / 2] = room_door;
		room->box[room_h - 1][(room_w / 2) - 1] = room_door;
	}
	if (room->right)
	{
		room->box[room_h / 2][room_w - 1] = room_door;
		room->box[(room_h / 2) - 1][room_w - 1] = room_door;
	}
	if (room->left)
	{
		room->box[room_h / 2][0] = room_door;
		room->box[(room_h / 2) - 1][0] = room_door;
	}
}

void level::draw(room * room) 
{
	for (unsigned __int8 i = 0; i < room_h; i++)
	{
		for (unsigned __int8 j = 0; j < room_w; j++)
		{
			printf("%c", room->box[i][j]) ;
		}	printf("\n");
	}printf("\n");
}

level::pos::pos(__int8 _h, __int8 _w, room * _link) 
{
	// ���� ���������� ������ �� ������ ������� �����, �� ��������� ��� ������ �����, ����� ��������� ����������� ��������� ������   
	(h <= level_size) ? h = _h : h = level_size / 2;
	(w <= level_size) ? w = _w : w = level_size / 2; 	// ���������� ��� ������
	link = _link;
}

level::pos level::getnextpos() // ���������� ��������� ����� ��������� ��� ����������
{
	pos next = nextgen.front(); // ����������� ������ � ������ �����
	nextgen.pop_front(); // ������� �� �� ������
	return next; // ������� �� �������
}

void level::draw() // ������ ����� � �������
{
	for (__int8 i = 0; i < level_size; i++)
	{
		for (__int8 j = 0; j < level_size; j++)
		{
			//��� _level'a �������� ��������� [1/0]  ��� ������� ������ ������ 1 - room_d(�������) 0 - empty_d(�������)
			printf("%c", box[i][j] ? room_wall : room_floor);
		}
		printf("\n");
	}
	printf("\n");
}

bool level::create(__int8 h, __int8 w) 
{
	srand(time(NULL) * rand()); // ���������� ��� ��������� ������ ��������� �����
	if (error > 2 || rand() % 2)
	{
		box[h][w] = NORMAL; // ��������� ������� ��� ������������, ������������ ����� �� 0{false} �� 1{true}

		neighbors++; // �������� ����� ������� ��� �������� ���� ������
		error = rand() % 2; // �������� ����������� ��������� ��� ����� �������� XD
							// ������ ������������ ������������� ��� ������������ ���������, �������� ���� ������ �������� error
		return true;
	}
	error++; // �������� ����� �������� ��������
	return false;
}

void level::generation() 
{

	if (nextgen.size()) // ��� �� ������ �������
	{
		pos next = getnextpos(); // ������� ������ ��������� �� ����������
		neighbors = 0; // ������� ����� �������, �.�. �������� ����� ��������

					   /* ��������� ���������� �����, ���� ����� ������������(+) ������ ��� "�����"(###)
					   //
					   //		###
					   //		 +
					   //																				*/
		if (box[next.h - 1][next.w] == EMPTY && box[next.h - 1][next.w - 1] == EMPTY && box[next.h - 1][next.w + 1] == EMPTY)
		{
			//create(next.h - 1, next.w); // � ���������� �������� ����� ������������� ��� next
			if (create(next.h - 1, next.w))
			{
				room * up = new room(NORMAL); // ������� ����� �������
				// ��������� ���� � ��� � �������
				next.link->up = up;
				up->down = next.link;

				build(next.link->up); // ���������� ����� � ������ ��������� �������

				nextgen.push_back(pos(next.h - 1, next.w, up)); // ������� �� � ������� �� ����������
			}
		}

		/* ��������� ���������� ����, ���� ����� ������������(+) ������ ��� "�����"(###)
		//
		//
		//		 +
		//		###																		*/
		if (box[next.h + 1][next.w] == EMPTY && box[next.h + 1][next.w - 1] == EMPTY && box[next.h + 1][next.w + 1] == EMPTY)
		{
			//create(next.h + 1, next.w); // � ���������� �������� ����� ������������� ��� next
			if (create(next.h + 1, next.w))
			{
				room * down = new room(NORMAL); // ������� ����� �������
				// ��������� ���� � ��� � �������
				next.link->down = down;
				down->up = next.link;

				build(next.link->down); // ���������� ����� � ������ ��������� �������

				nextgen.push_back(pos(next.h + 1, next.w, down)); // ������� �� � ������� �� ����������
			}
		}

		/* ��������� ���������� �����, ���� ����� ������������(+) ������ ��� "�����"(###)
		//
		//		#
		//		# +
		//		#																		*/
		if (box[next.h][next.w - 1] == EMPTY && box[next.h - 1][next.w - 1] == EMPTY && box[next.h + 1][next.w - 1] == EMPTY)
		{
			//create(next.h, next.w - 1); // � ���������� �������� ����� ������������� ����� �� next
			if (create(next.h, next.w - 1))
			{ 
				room * left = new room(NORMAL); // ������� ����� �������
				// ��������� ���� � ��� � �������
				next.link->left = left;
				left->right = next.link;

				build(next.link->left); // ���������� ����� � ������ ��������� �������

				nextgen.push_back(pos(next.h, next.w - 1, left)); // ������� �� � ������� �� ����������
			}
		}

		/* ��������� ���������� ������, ���� ����� ������������(+) ������ ��� "�����"(###)
		//
		//			#
		//		  + #
		//			#																	*/
		if (box[next.h][next.w + 1] == EMPTY && box[next.h - 1][next.w + 1] == EMPTY && box[next.h + 1][next.w + 1] == EMPTY)
		{
			//create(next.h, next.w + 1); // � ���������� �������� ����� ������������� ������ �� next
			if (create(next.h, next.w + 1))
			{
				room * right = new room(NORMAL); // ������� ����� �������
				// ��������� ���� � ��� � �������
				next.link->right = right;
				right->left = next.link;

				build(next.link->right); // ���������� ����� � ������ ��������� �������

				nextgen.push_back(pos(next.h, next.w + 1, right)); // ������� �� � ������� �� ����������
			}
		}
	
		build(next.link); // ���������� ����� � ������ ��������� �������
	}
}

level::level() 
{
	// ��������� �����
	for (__int8 i = 0; i < level_size; i++)
	{
		for (__int8 j = 0; j < level_size; j++)
		{
			box[i][j] = EMPTY; // ������� �����
		}
	}

	box[level_size / 2][level_size / 2] = NORMAL; // �������� ����������� �������
	nextgen.push_back(pos(level_size / 2, level_size / 2, center)); // ������ ����� �������� ��� ��������� ������� ��� � �������
}
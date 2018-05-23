#include "generation.h"

level::room::room(RoomType _T)
{
	type = _T;
}

level::pos::pos(__int8 _h, __int8 _w, room * _link) 
{
	// ���� ���������� ������ �� ������ ������� �����, �� ��������� ��� ������ �����, ����� ��������� ����������� ��������� ������   
	(h <= level_size) ? h = _h : h = level_size / 2;
	(w <= level_size) ? w = _w : w = level_size / 2; 	// ���������� ��� ������
	link = _link;
}

level::pos level::getnextpos()
{
	pos next = nextgen.front(); // ����������� ������ � ������ �����
	nextgen.pop_front(); // ������� �� �� ������
	return next; // ������� �� �������
}

void level::draw() 
{
	for (__int8 i = 0; i < level_size; i++)
	{
		for (__int8 j = 0; j < level_size; j++)
		{
			//��� _level'a �������� ��������� [1/0]  ��� ������� ������ ������ 1 - room_d(�������) 0 - empty_d(�������)
			printf("%c", box[i][j] ? room_d : empty_d);
		}
		printf("\n");
	}
	printf("\n");
}

bool level::create(__int8 h, __int8 w) 
{
	srand(time(NULL) * rand() % 100); // ���������� ��� ��������� ������ ��������� �����
	if (error > 2 || rand() % 2)
	{
		box[h][w] = true; // ��������� ������� ��� ������������, ������������ ����� �� 0{false} �� 1{true}

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
		if (box[next.h - 1][next.w] == false && box[next.h - 1][next.w - 1] == false && box[next.h - 1][next.w + 1] == false)
		{
			//create(next.h - 1, next.w); // � ���������� �������� ����� ������������� ��� next
			if (create(next.h - 1, next.w))
			{
				room * up = new room(NORMAL); // ������� ����� �������
				// ��������� ���� � ��� � �������
				next.link->up = up;
				up->down = next.link;

				nextgen.push_back(pos(next.h - 1, next.w, up)); // ������� �� � ������� �� ����������
			}
		}

		/* ��������� ���������� ����, ���� ����� ������������(+) ������ ��� "�����"(###)
		//
		//
		//		 +
		//		###																		*/
		if (box[next.h + 1][next.w] == false && box[next.h + 1][next.w - 1] == false && box[next.h + 1][next.w + 1] == false)
		{
			//create(next.h + 1, next.w); // � ���������� �������� ����� ������������� ��� next
			if (create(next.h + 1, next.w))
			{
				room * down = new room(NORMAL); // ������� ����� �������
				// ��������� ���� � ��� � �������
				next.link->down = down;
				down->up = next.link;

				nextgen.push_back(pos(next.h + 1, next.w, down)); // ������� �� � ������� �� ����������
			}
		}

		/* ��������� ���������� �����, ���� ����� ������������(+) ������ ��� "�����"(###)
		//
		//		#
		//		# +
		//		#																		*/
		if (box[next.h][next.w - 1] == false && box[next.h - 1][next.w - 1] == false && box[next.h + 1][next.w - 1] == false)
		{
			//create(next.h, next.w - 1); // � ���������� �������� ����� ������������� ����� �� next
			if (create(next.h, next.w - 1))
			{ 
				room * left = new room(NORMAL); // ������� ����� �������
				// ��������� ���� � ��� � �������
				next.link->left = left;
				left->right = next.link;

				nextgen.push_back(pos(next.h, next.w - 1, left)); // ������� �� � ������� �� ����������
			}
		}

		/* ��������� ���������� ������, ���� ����� ������������(+) ������ ��� "�����"(###)
		//
		//			#
		//		  + #
		//			#																	*/
		if (box[next.h][next.w + 1] == false && box[next.h - 1][next.w + 1] == false && box[next.h + 1][next.w + 1] == false)
		{
			//create(next.h, next.w + 1); // � ���������� �������� ����� ������������� ������ �� next
			if (create(next.h, next.w + 1))
			{
				room * right = new room(NORMAL); // ������� ����� �������
				// ��������� ���� � ��� � �������
				next.link->right = right;
				right->left = next.link;

				nextgen.push_back(pos(next.h, next.w + 1, right)); // ������� �� � ������� �� ����������
			}
		}
	}
}

level::level() 
{
	// ��������� �����
	for (__int8 i = 0; i < level_size; i++)
	{
		for (__int8 j = 0; j < level_size; j++)
		{
			box[i][j] = false; // ������� �����
		}
	}

	box[level_size / 2][level_size / 2] = true; // �������� ����������� �������
	nextgen.push_back(pos(level_size / 2, level_size / 2, center)); // ������ ����� �������� ��� ��������� ������� ��� � �������
}
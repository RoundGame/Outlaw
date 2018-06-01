#include "generation.h"

level::room::room(RoomType _T)
{
	srand(time(0) * rand());
	type = _T;
	up = down = right = left = nullptr;

	////////////////// Пустая коробка уровня /////////////////////////////////
	for (unsigned __int8 i = 1; i < room_h - 1; i++) // Очищаем пол
	{
		for (unsigned __int8 j = 1; j < room_w - 1; j++)
		{
			box[i][j] = room_floor;
		}
	}

	int k = 0;
	for (unsigned __int8 i = 1 + safe_indent; i < room_h - 1 - safe_indent; i++) // Генерируем положение препятствий
	{
		for (unsigned __int8 j = 1 + safe_indent; j < room_w - 1 - safe_indent; j++)
		{
			if (rand() % wall_frequency == 0)
			{
				box[i][j] = room_wall;
			}
		}
	}

	for (unsigned __int8 i = 0; i < enemy_size; i++) // Генерируем положение врагов
	{
		box[rand() % room_h][rand() % room_w] = room_enemy;
	}
	if (rand() % prob_gift == 0)
	{
		box[rand() % (room_h-1) + 1][rand() % (room_w - 1) + 1 ] = room_gift; // Подарок в комнату
	}

	for (int i = 0; i < room_h; i++) // Рисуем стены ( левая и правая)
	{
		box[i][0] = room_wall;
		box[i][room_w - 1] = room_wall;
	}

	for (int j = 1; j < room_w - 1; j++) // Рисуем стены ( верхняя и нижняя)
	{
		box[0][j] = room_wall;
		box[room_h - 1][j] = room_wall;
	}
	//////////////////////////////////////////////////////////////////////////
}

void level::build(room * room) // Генерирует содержимое комнаты
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
	// Если полученная высота не больше размера карты, то присвойть эту высоту точке, иначе присвойть максимально доступную высоту   
	(h <= level_size) ? h = _h : h = level_size / 2;
	(w <= level_size) ? w = _w : w = level_size / 2; 	// Аналогично для ширины
	link = _link;
}

level::pos level::getnextpos() // Возвращает следующую точку доступную для расширения
{
	pos next = nextgen.front(); // Скопировать первую в списке точку
	nextgen.pop_front(); // Удалить ее из списка
	return next; // Вернуть по запросу
}

void level::draw() // Рисует карту в консоли
{
	for (__int8 i = 0; i < level_size; i++)
	{
		for (__int8 j = 0; j < level_size; j++)
		{
			//Для _level'a возможны состояния [1/0]  для которых рисуем символ 1 - room_d(комната) 0 - empty_d(пустота)
			printf("%c", box[i][j] ? room_wall : room_floor);
		}
		printf("\n");
	}
	printf("\n");
}

bool level::create(__int8 h, __int8 w) 
{
	srand(time(NULL) * rand()); // Необходимо для генерации псевдо случайных чисел
	if (error > 2 || rand() % 2)
	{
		box[h][w] = NORMAL; // Обозначим комнату как существующую, сгенерировав число от 0{false} до 1{true}

		neighbors++; // Увеличим число соседей для родителя этой комнты
		error = rand() % 2; // Исключим возможность остановки еще одним рандомом XD
							// Рандом предназначен исключительно для разнообрация генерации, возможно было просто обнулить error
		return true;
	}
	error++; // Увеличим число промахов создания
	return false;
}

void level::generation() 
{

	if (nextgen.size()) // Для не пустой очереди
	{
		pos next = getnextpos(); // Получим нового кандидата на расширение
		neighbors = 0; // Обнулим число соседей, т.к. поступил новый родитель

					   /* Проверяем расширение вверх, если перед генерируемой(+) точкой нет "стены"(###)
					   //
					   //		###
					   //		 +
					   //																				*/
		if (box[next.h - 1][next.w] == EMPTY && box[next.h - 1][next.w - 1] == EMPTY && box[next.h - 1][next.w + 1] == EMPTY)
		{
			//create(next.h - 1, next.w); // В расширение отправим точку расположенную над next
			if (create(next.h - 1, next.w))
			{
				room * up = new room(NORMAL); // Создаем новую комнату
				// Указываем путь к ней и обратно
				next.link->up = up;
				up->down = next.link;

				build(next.link->up); // Построение карты с учетом возможных выходов

				nextgen.push_back(pos(next.h - 1, next.w, up)); // Запишем ее в очередь на расширение
			}
		}

		/* Проверяем расширение вниз, если перед генерируемой(+) точкой нет "стены"(###)
		//
		//
		//		 +
		//		###																		*/
		if (box[next.h + 1][next.w] == EMPTY && box[next.h + 1][next.w - 1] == EMPTY && box[next.h + 1][next.w + 1] == EMPTY)
		{
			//create(next.h + 1, next.w); // В расширение отправим точку расположенную под next
			if (create(next.h + 1, next.w))
			{
				room * down = new room(NORMAL); // Создаем новую комнату
				// Указываем путь к ней и обратно
				next.link->down = down;
				down->up = next.link;

				build(next.link->down); // Построение карты с учетом возможных выходов

				nextgen.push_back(pos(next.h + 1, next.w, down)); // Запишем ее в очередь на расширение
			}
		}

		/* Проверяем расширение влево, если перед генерируемой(+) точкой нет "стены"(###)
		//
		//		#
		//		# +
		//		#																		*/
		if (box[next.h][next.w - 1] == EMPTY && box[next.h - 1][next.w - 1] == EMPTY && box[next.h + 1][next.w - 1] == EMPTY)
		{
			//create(next.h, next.w - 1); // В расширение отправим точку расположенную слева от next
			if (create(next.h, next.w - 1))
			{ 
				room * left = new room(NORMAL); // Создаем новую комнату
				// Указываем путь к ней и обратно
				next.link->left = left;
				left->right = next.link;

				build(next.link->left); // Построение карты с учетом возможных выходов

				nextgen.push_back(pos(next.h, next.w - 1, left)); // Запишем ее в очередь на расширение
			}
		}

		/* Проверяем расширение вправо, если перед генерируемой(+) точкой нет "стены"(###)
		//
		//			#
		//		  + #
		//			#																	*/
		if (box[next.h][next.w + 1] == EMPTY && box[next.h - 1][next.w + 1] == EMPTY && box[next.h + 1][next.w + 1] == EMPTY)
		{
			//create(next.h, next.w + 1); // В расширение отправим точку расположенную справа от next
			if (create(next.h, next.w + 1))
			{
				room * right = new room(NORMAL); // Создаем новую комнату
				// Указываем путь к ней и обратно
				next.link->right = right;
				right->left = next.link;

				build(next.link->right); // Построение карты с учетом возможных выходов

				nextgen.push_back(pos(next.h, next.w + 1, right)); // Запишем ее в очередь на расширение
			}
		}
	
		build(next.link); // Построение карты с учетом возможных выходов
	}
}

level::level() 
{
	// Обнуление карты
	for (__int8 i = 0; i < level_size; i++)
	{
		for (__int8 j = 0; j < level_size; j++)
		{
			box[i][j] = EMPTY; // видимой части
		}
	}

	box[level_size / 2][level_size / 2] = NORMAL; // Создадим центральную комнату
	nextgen.push_back(pos(level_size / 2, level_size / 2, center)); // Теперь центр доступен для генерации занесем его в очередь
}
#include "generation.h"

level::room::room(RoomType _T)
{
	type = _T;
}

level::pos::pos(__int8 _h, __int8 _w, room * _link) 
{
	// Если полученная высота не больше размера карты, то присвойть эту высоту точке, иначе присвойть максимально доступную высоту   
	(h <= level_size) ? h = _h : h = level_size / 2;
	(w <= level_size) ? w = _w : w = level_size / 2; 	// Аналогично для ширины
	link = _link;
}

level::pos level::getnextpos()
{
	pos next = nextgen.front(); // Скопировать первую в списке точку
	nextgen.pop_front(); // Удалить ее из списка
	return next; // Вернуть по запросу
}

void level::draw() 
{
	for (__int8 i = 0; i < level_size; i++)
	{
		for (__int8 j = 0; j < level_size; j++)
		{
			//Для _level'a возможны состояния [1/0]  для которых рисуем символ 1 - room_d(комната) 0 - empty_d(пустота)
			printf("%c", box[i][j] ? room_d : empty_d);
		}
		printf("\n");
	}
	printf("\n");
}

bool level::create(__int8 h, __int8 w) 
{
	srand(time(NULL) * rand() % 100); // Необходимо для генерации псевдо случайных чисел
	if (error > 2 || rand() % 2)
	{
		box[h][w] = true; // Обозначим комнату как существующую, сгенерировав число от 0{false} до 1{true}

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
		if (box[next.h - 1][next.w] == false && box[next.h - 1][next.w - 1] == false && box[next.h - 1][next.w + 1] == false)
		{
			//create(next.h - 1, next.w); // В расширение отправим точку расположенную над next
			if (create(next.h - 1, next.w))
			{
				room * up = new room(NORMAL); // Создаем новую комнату
				// Указываем путь к ней и обратно
				next.link->up = up;
				up->down = next.link;

				nextgen.push_back(pos(next.h - 1, next.w, up)); // Запишем ее в очередь на расширение
			}
		}

		/* Проверяем расширение вниз, если перед генерируемой(+) точкой нет "стены"(###)
		//
		//
		//		 +
		//		###																		*/
		if (box[next.h + 1][next.w] == false && box[next.h + 1][next.w - 1] == false && box[next.h + 1][next.w + 1] == false)
		{
			//create(next.h + 1, next.w); // В расширение отправим точку расположенную под next
			if (create(next.h + 1, next.w))
			{
				room * down = new room(NORMAL); // Создаем новую комнату
				// Указываем путь к ней и обратно
				next.link->down = down;
				down->up = next.link;

				nextgen.push_back(pos(next.h + 1, next.w, down)); // Запишем ее в очередь на расширение
			}
		}

		/* Проверяем расширение влево, если перед генерируемой(+) точкой нет "стены"(###)
		//
		//		#
		//		# +
		//		#																		*/
		if (box[next.h][next.w - 1] == false && box[next.h - 1][next.w - 1] == false && box[next.h + 1][next.w - 1] == false)
		{
			//create(next.h, next.w - 1); // В расширение отправим точку расположенную слева от next
			if (create(next.h, next.w - 1))
			{ 
				room * left = new room(NORMAL); // Создаем новую комнату
				// Указываем путь к ней и обратно
				next.link->left = left;
				left->right = next.link;

				nextgen.push_back(pos(next.h, next.w - 1, left)); // Запишем ее в очередь на расширение
			}
		}

		/* Проверяем расширение вправо, если перед генерируемой(+) точкой нет "стены"(###)
		//
		//			#
		//		  + #
		//			#																	*/
		if (box[next.h][next.w + 1] == false && box[next.h - 1][next.w + 1] == false && box[next.h + 1][next.w + 1] == false)
		{
			//create(next.h, next.w + 1); // В расширение отправим точку расположенную справа от next
			if (create(next.h, next.w + 1))
			{
				room * right = new room(NORMAL); // Создаем новую комнату
				// Указываем путь к ней и обратно
				next.link->right = right;
				right->left = next.link;

				nextgen.push_back(pos(next.h, next.w + 1, right)); // Запишем ее в очередь на расширение
			}
		}
	}
}

level::level() 
{
	// Обнуление карты
	for (__int8 i = 0; i < level_size; i++)
	{
		for (__int8 j = 0; j < level_size; j++)
		{
			box[i][j] = false; // видимой части
		}
	}

	box[level_size / 2][level_size / 2] = true; // Создадим центральную комнату
	nextgen.push_back(pos(level_size / 2, level_size / 2, center)); // Теперь центр доступен для генерации занесем его в очередь
}
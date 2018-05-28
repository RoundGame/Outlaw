#pragma once
#include <ctime>
#include <list>
#include "config.h"

#include <windows.h>
using namespace std;
/*	Тип комнаты
EMPTY	- Пустая,
NORMAL	- Обычная(не изучена),
SHOP	- Магазин,
BOSS	- Покои босса,
SECRET	- Тайник	*/
enum RoomType
{
	EMPTY, NORMAL, SHOP, BOSS, SECRET
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
		room(RoomType _T); // Создает комнату типа _T
	};
	struct pos // точка на карте
	{
		__int8 h; // ширина
		__int8 w; // высота
		room *link = nullptr; // указатель на комнату в этой точке, по дефолту комнаты нет
		
		pos(__int8 _h, __int8 _w, room * _link); // Создает точку на карте в позиции _h(высота) _w(шириша) _link(указатель на комнату)
	};
	
	room * center = new room(EMPTY); // Центральныя комната
	room * current = center;
	list<pos> nextgen; // Очередь точек в которых возможно расширение карты
	
	__int8 neighbors = 0; // Число соседей
	__int8 rooms = 0; // Число соседей
	__int8 error = 0; // Количество несгенерированных комнат подряд, необходимо для исключения одиночной комнаты
	char box[level_size][level_size]; // карта [высота][ширина]
	pos getnextpos(); // Процедура получения из очереди следующей точки

	void draw(); // Вывод карты в консоли
	void draw(room * room); // Рисует комнату в консоли
	bool create(__int8 h, __int8 w); /* С шансом в положительном исходе обозначае точку (h,w) как имеющую комнату,
										увеличивает число соседей для ее родителя и возвращает true, иначе false*/
	void generation(); /*Если список точек доступных к расширению карты не пустой, то
					   получает нового кандидата на расширение, в доступных к генерации
					   направлениях создает ответвления(новые комнаты) и записывает их 
					   в список точек доступных к расширению карты*/
	void build(room * room); // Генерирует содержимое комнаты

	level(); // Конструктор создания пустой карты с начальной комнатой в центре
};
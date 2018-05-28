#pragma once
#include "magnitudes.h"
#define room_floor ' '
#define room_wall '#'
#define room_door '*'

// Частота вызова функции изменения позиции персонажей
const int timer_update = 15;

// Частота вызова функции изменения анимации персонажей
const int timer_animation = 50;

// Массив доступных игровых клавиш
static Key key[gamekey_size];

// Соотношение строн
static	int win_width = 16,
			win_height = 9;

// Генерация
static const __int8 level_size = 7; // Размер "коробки" в пределах которой генерируется комната
static const __int8 room_h = 12; // Длина комнат
static const __int8 room_w = 22; // Ширина комнат
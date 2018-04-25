#pragma once
#include "magnitudes.h"

// Частота вызова функции изменения позиции персонажей
const int timer_update = 15;

// Частота вызова функции изменения анимации персонажей
const int timer_animation = 50;

// Массив доступных игровых клавиш
static Key key[sizeof(gamekey)];

// Соотношение строн
static	int win_width = 16,
			win_height = 9;
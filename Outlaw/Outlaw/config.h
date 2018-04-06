#pragma once
#include "magnitudes.h"

// Частота вызова функции изменения позиции персонажей
static int timer_update = 20; 

// Частота вызова функции изменения анимации персонажей
static int timer_animation = 100; 

// Обращение к клавишам управления
enum gamekey { LEFT, RIGHT, UP, DOWN };

// Количество игровых клавиш
const int key_length = sizeof(gamekey);
static Key key[key_length];

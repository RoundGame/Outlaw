#pragma once
#include "magnitudes.h"
static int timer_update = 20; // Частота вызова функции изменения позиции персонажей
static int timer_animation = 100; // Частота вызова функции изменения анимации персонажей

/*
static int key_left = 97;
static int key_right = 100;
static int key_up = 119;
static int key_down = 115;
*/

const int key_length = 4;
static Key key[key_length];

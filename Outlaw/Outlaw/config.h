#pragma once
#include "magnitudes.h"

#define room_floor ' '
#define room_wall '#'
#define room_door '*'
#define room_enemy 'E'
#define room_gift '?'

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
static const unsigned __int8 level_size = 7; // Размер "коробки" в пределах которой генерируется комната
static const unsigned __int8 room_h = 12; // Длина комнат
static const unsigned __int8 room_w = 20; // Ширина комнат

static const unsigned __int8 enemy_size = 5; // Кол-во врагов на уровень
static const unsigned __int8 prob_gift = 7; // Вероятность спавна подарка на уровне 1 к prob_gift
static const unsigned __int8 wall_frequency = 20; // Плотность генерации стен wall_frequency - вероятность к 1 спавна стены на точке 
static const unsigned __int8 safe_indent = 1; // Отступ от стен(в блоках) в котором не генерируются препятствия
static const unsigned __int8 wall_count = 90; // Количество комнат на уровне

// Меню
static const Vector menu_button_size = Vector(0.6, 0.1) ; // Размер кнопок в главном меню
static const Vector menu_text_size = Vector(0.8, 0.12); // Размер заголовков в главном меню
static const Vector menu_text_position = Vector(-0.7, 0.3); // Позиция заголовков в главном меню
static const float menu_button_ident = 0.1f; // Отступ кнопок от от левой границы экрана
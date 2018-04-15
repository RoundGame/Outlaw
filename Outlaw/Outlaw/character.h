#pragma once
#include <GL/glut.h>
#include "magnitudes.h"

struct Physical_component
{
	double	Speed = 0.4; // Множитель скорости изменения координат (скорость передвижения)
	double	Boost = 4; //Коэффициент ускорения. Если он большой, моментально ускоряемся и останавливаемся (обычная поверхность), если маленький, то медленно ускоряемся и медленно останавливаемся (например лёд)
	double Angle = 0; //Угол поворота относительно вектора с координатами (0, 1)
	Vector	Position, // Позиция
			Velocity, // Скорость
			Acceleration; // Ускорение

	void Update(bool isAcceleration);
};

struct Object
{
	bool isExist = false;
	Sprite Body;
	Physical_component Physics;
};

struct Character
{
	/*Механикуа перерабатывается*/ double	Direction = 3;	// Началное направление персонажа (3 - смотрит вперед) 
	/*Механикуа перерабатывается*/ double	CurrentFrame = 0;	// Текуший кадр анимации
	/*Механикуа перерабатывается*/ bool		CurrentAnimation = 0;	// Текущая анимация 0 - стоим, 1 - идем
	
	Physical_component Physics; // Физический компонент персонажа (Позция, Скорость, Ускорение)
	Sprite Leg, Body;	// Текстура персонажа

	void Animation();	// Анимаций персонажа
	void Update();		// Подсчет координат перемещения персонажа
	void Draw();		// Отрисовка персонажа по своим координатам в мире.
};
#pragma once
#include "magnitudes.h"

struct Character
{
	double	speed = 0.02; // Множитель скорости изменения координат (скорость передвижения)
	double	CurrentFrame = 0; // Текуший кадр анимации
	bool	CurrentAnimation = 0;	//Текущая анимация 0 - стоим, 1 - идем
	Vector	Position, // Позиция
			Velocity, // Скорость
			Acceleration; // Ускорение
	void Animation(int frame); // Анимаций персонажа
	void Update();	  // Подсчет координат перемещения персонажа
};

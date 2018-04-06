#pragma once
#include <GL/glut.h>
#include "magnitudes.h"

struct Character
{
	double  Direction = 3; // Началное направление персонажа (3 - смотрит вперед)
	double	speed = 0.4; // Множитель скорости изменения координат (скорость передвижения)
	double	boost = 4; //Коэффициент ускорения. Если он большой, моментально ускоряемся и останавливаемся (обычная поверхность), если маленький, то медленно ускоряемся и медленно останавливаемся (например лёд)
	double	CurrentFrame = 0; // Текуший кадр анимации
	bool	CurrentAnimation = 0;	//Текущая анимация 0 - стоим, 1 - идем
	unsigned int Texture; // Текстура персонажа
	Vector	Position, // Позиция
			Velocity, // Скорость
			Acceleration; // Ускорение
	
	void Animation(int frame); // Анимаций персонажа
	void Update();	  // Подсчет координат перемещения персонажа
	void Draw();	// Отрисовка персонажа по своим координатам в мире.
};
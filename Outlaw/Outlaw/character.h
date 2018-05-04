#pragma once
#include <GL/glut.h>
#include "magnitudes.h"


struct Physical_component
{
	double	Speed = 0.2; // Множитель скорости изменения координат (скорость передвижения)
	double	Boost = 5; //Коэффициент ускорения. Если он большой, моментально ускоряемся и останавливаемся (обычная поверхность), если маленький, то медленно ускоряемся и медленно останавливаемся (например лёд)
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

struct Static_Object
{
	bool isExist = false;
	Sprite Body;
	Vector Position;
};

struct Character
{
	bool isAttack = false;
	bool isKick = false;
	double HP = 100;
	double Knock_Back = 0.6;
	/*Механикуа перерабатывается*/ double	Direction = 0;	// Началное направление персонажа (3 - смотрит вперед) 
	/*Механикуа перерабатывается*/ double	Legs_CurrentFrame = 0;	// Текуший кадр анимации
	double	Attack_CurrentFrame = 0;
	
	Physical_component Physics; // Физический компонент персонажа (Позция, Скорость, Ускорение)
	Sprite Legs, Body, Death, Attack;	// Текстура персонажа

	void Animation();	// Анимаций персонажа
	void Target_To(Vector Target, Vector Window_Render_Size); // Установить угол(Physics.Angle) направления в сторону обьекта цели
	void Set_Legs_Direction();
	void Draw();		// Отрисовка персонажа по своим координатам в мире.
	void Use_Collisions(Static_Object Obj[], int obj_count);
};
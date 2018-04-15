#pragma once
#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include "key.h"
#include <cstdio>
#include <soil.h>
#include <cmath>

// Обращение к клавишам управления
enum gamekey { LEFT, RIGHT, UP, DOWN };

struct Vector
{
	double	X,
			Y;
	Vector()
	{
		X = 0;
		Y = 0;
	}
	Vector(double x, double y)
	{
		X = x;
		Y = y;
	}

	/*Получение длины вектора*/
	double GetLength()
	{
		double len = sqrt(X * X + Y * Y);
		return len;
	}
	/*Нормализация вектора*/
	Vector GetNormalize()
	{
		if (GetLength() == 0)
			return Vector();
		double x = X / GetLength();
		double y = Y / GetLength();
		return Vector(x, y);
	}
};

struct Key
{
	int Nominal;
	bool isPressed;
	Key()
	{
		Nominal = KEY_UNKNOWN;
		isPressed = false;
	}
};

struct Room
{
	unsigned int Texture;
	float Size;
};

struct Entity
{
	float Size;
	unsigned int Texture;
	bool Barrier; // Является преградой? 
	Vector Position;
	Entity()
	{
		Size = 0.1f;
		Barrier = false;
	}
};

struct Sprite
{
	unsigned int Texture;

	int AnimationSize;	// Возвращает Кол - во кадров анимаций		(условие текстуры - кадры расположены слеава на право с размером 32x32)
	int AnimationNumber; // Возвращает Кол - во анимаций в текстуре (условие текстуры - кадры расположены слеава на право с размером 32x32)
	const int FrameSize = 32;	// Размер анимационного кадра

	void Load(const char Name[]) // Загрузка текстуры
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		int width, height, channels;
		unsigned char* image = SOIL_load_image(Name, &width, &height, &channels, SOIL_LOAD_RGBA);

		AnimationSize = width / FrameSize;		// Кол - во кадров анимаций
		AnimationNumber = height / FrameSize;	// Кол - во анимаций в текстуре

		if (image == 0) // Проверяем удалось ли загрузить изображение
			printf("InitTexture ERROR : %s \n", Name); // При ошибке показываем название файла, загрузка которого не удалась
		else
		{
			glGenTextures(1, &Texture); // Генерация текстуры			
			glBindTexture(GL_TEXTURE_2D, Texture);// Установка параметров
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Устанавливаем пиксельный фильтр
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // На любой размер изображения 
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);// Создание миникарты
		}
	}
};

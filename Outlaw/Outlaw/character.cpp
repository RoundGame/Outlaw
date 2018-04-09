#include "character.h"

// Функция прокрутки анимации персонажа, принемаемый параметр frame - максимальное кол-во кадров анимации
void Character::Animation(int frame)
{
	if (CurrentAnimation)
	{
		CurrentFrame++;
		if (CurrentFrame > frame && CurrentFrame < 0) //В анимации frame + 1 кадров, поэтому сбрасываем счетчик на 0, как только он перевалил за frame
			CurrentFrame = 0;
	}
}

void Character::Update() 
{
	Move.Velocity.X += (Move.Acceleration.X * 0.01 - Move.Velocity.X * 0.1) * Boost; //Добавлем к вектору скорости вектор ускорения. boost - коэффициент ускорения
	Move.Velocity.Y += (Move.Acceleration.Y * 0.01 - Move.Velocity.Y * 0.1) * Boost; //Вычитаем из вектора ускорения вектор текущей скорости, тем самым имитируем трение
	Move.Position.X += Move.Velocity.X * Speed; //Добавляем к вектору текущей позиции вектор текущей скорости
	Move.Position.Y += Move.Velocity.Y * Speed;
	if (Move.Velocity.GetLength() > 0.01 || Move.Velocity.GetLength() < -0.01) //Если есть скорость то
	{
		CurrentAnimation = true; //Включаем анимацию передвижения
	}
	else //Иначе
	{
		CurrentFrame = 0; //Текущий кадр - нулевой (стоим)
		CurrentAnimation = false; //Текущая анимация - нулевая (стоим)
	}
}

void Character::Draw()
{
	glBindTexture(GL_TEXTURE_2D, Texture); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(CurrentFrame / texture_frame, (Direction - 1) / 8/*кол-во направлений*/ + 0.125); glVertex2f(-0.25 + Move.Position.X, -0.25 + Move.Position.Y);
	glTexCoord2f(CurrentFrame / texture_frame, (Direction - 1) / 8/*кол-во направлений*/); glVertex2f(-0.25 + Move.Position.X, 0.25 + Move.Position.Y);
	glTexCoord2f(CurrentFrame / texture_frame + 1/texture_frame/*1/кол-во кадров (ширина кадра) */, (Direction - 1) / 8/*кол-во направлений*/); glVertex2f(0.25 + Move.Position.X, 0.25 + Move.Position.Y);
	glTexCoord2f(CurrentFrame / texture_frame + 1 / texture_frame, (Direction - 1) / 8/*кол-во направлений*/ + 0.125); glVertex2f(0.25 + Move.Position.X, -0.25 + Move.Position.Y);
	glEnd(); // Конец обьекта рисуемого треугольниками
}

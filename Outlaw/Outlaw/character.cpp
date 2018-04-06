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
	Velocity.X += (Acceleration.X * 0.01 - Velocity.X * 0.1) * boost; //Добавлем к вектору скорости вектор ускорения. boost - коэффициент ускорения
	Velocity.Y += (Acceleration.Y * 0.01 - Velocity.Y * 0.1) * boost; //Вычитаем из вектора ускорения вектор текущей скорости, тем самым имитируем трение
	Position.X += Velocity.X * speed; //Добавляем к вектору текущей позиции вектор текущей скорости
	Position.Y += Velocity.Y * speed;
	if (Velocity.GetLength() > 0.01 || Velocity.GetLength() < -0.01) //Если есть скорость то
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
	glTexCoord2f(CurrentFrame / 8, (Direction - 1) / 8 + 0.125); glVertex2f(-0.25 + Position.X, -0.25 + Position.Y);
	glTexCoord2f(CurrentFrame / 8, (Direction - 1) / 8); glVertex2f(-0.25 + Position.X, 0.25 + Position.Y);
	glTexCoord2f(CurrentFrame / 8 + 0.125, (Direction - 1) / 8); glVertex2f(0.25 + Position.X, 0.25 + Position.Y);
	glTexCoord2f(CurrentFrame / 8 + 0.125, (Direction - 1) / 8 + 0.125); glVertex2f(0.25 + Position.X, -0.25 + Position.Y);
	glEnd(); // Конец обьекта рисуемого треугольниками
}

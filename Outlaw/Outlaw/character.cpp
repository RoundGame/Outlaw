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
	Move.Acceleration = Move.Acceleration.GetNormalize(); //Нормализуем полученный вектор ускорения
	if (Move.Acceleration.GetLength() != 0) //Если длина вектора равна нулю, то мы стоим и не нужно считать новое направление
	{
		if (Move.Acceleration.Y >= 0) // Считаем направление персонажа при движении (u - вперед, l - влево, r - право) u, r, l, ur, ul
			Direction = round(-2 * Move.Acceleration.X + 3);
		else	// Считаем направление персонажа при движении (d - вниз, l - влево, r - право) d, dr, dl
			Direction = round(2 * Move.Acceleration.X + 7);
		// Записываем получившееся число в Direction
	}

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
	glBindTexture(GL_TEXTURE_2D, Texture1); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(CurrentFrame / texture_frame, (Direction - 1) / 8/*кол-во направлений*/ + 0.125); glVertex2f(-0.3 + Move.Position.X, -0.3 + Move.Position.Y);
	glTexCoord2f(CurrentFrame / texture_frame, (Direction - 1) / 8/*кол-во направлений*/); glVertex2f(-0.3 + Move.Position.X, 0.3 + Move.Position.Y);
	glTexCoord2f(CurrentFrame / texture_frame + 1 / (double)texture_frame/*1/кол-во кадров (ширина кадра) */, (Direction - 1) / 8/*кол-во направлений*/); glVertex2f(0.3 + Move.Position.X, 0.3 + Move.Position.Y);
	glTexCoord2f(CurrentFrame / texture_frame + 1 / (double)texture_frame, (Direction - 1) / 8/*кол-во направлений*/ + 0.125); glVertex2f(0.3 + Move.Position.X, -0.3 + Move.Position.Y);
	glEnd(); // Конец обьекта рисуемого треугольниками

	glBindTexture(GL_TEXTURE_2D, Texture2); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(Move.Position.X, Move.Position.Y, 0);
	glRotated(-Angle * 180 / M_PI - 90, 0, 0, 1);
	glTranslated(-Move.Position.X, -Move.Position.Y, 0);
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.15 + Move.Position.X, -0.15 + Move.Position.Y);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.15 + Move.Position.X, 0.15 + Move.Position.Y);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.15 + Move.Position.X, 0.15 + Move.Position.Y);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.15 + Move.Position.X, -0.15 + Move.Position.Y);
	glEnd();
	glPopMatrix();
}

#include "character.h"

void Physical_component::Update(bool isAcceleration)
{
	if (isAcceleration)
	{
		Velocity.X += (Acceleration.X * 0.01 - Velocity.X * 0.1) * Boost; //Добавлем к вектору скорости вектор ускорения. boost - коэффициент ускорения
		Velocity.Y += (Acceleration.Y * 0.01 - Velocity.Y * 0.1) * Boost; //Вычитаем из вектора ускорения вектор текущей скорости, тем самым имитируем трение
	}
	Position.X += Velocity.X * Speed; //Добавляем к вектору текущей позиции вектор текущей скорости
	Position.Y += Velocity.Y * Speed;
}

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

	Physics.Acceleration = Physics.Acceleration.GetNormalize(); //Нормализуем полученный вектор ускорения
	if (Physics.Acceleration.GetLength() != 0) //Если длина вектора равна нулю, то мы стоим и не нужно считать новое направление
	{
		if (Physics.Acceleration.Y >= 0) // Считаем направление персонажа при движении (u - вперед, l - влево, r - право) u, r, l, ur, ul
			Direction = round(-2 * Physics.Acceleration.X + 3);
		else	// Считаем направление персонажа при движении (d - вниз, l - влево, r - право) d, dr, dl
			Direction = round(2 * Physics.Acceleration.X + 7);
		// Записываем получившееся число в Direction
	}

	if (Physics.Velocity.GetLength() > 0.01 || Physics.Velocity.GetLength() < -0.01) //Если есть скорость то
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
	glTexCoord2f(CurrentFrame / texture_frame, 0.125); glVertex2f(-0.3 + Physics.Position.X, -0.3 + Physics.Position.Y);
	glTexCoord2f(CurrentFrame / texture_frame, 0.0); glVertex2f(-0.3 + Physics.Position.X, 0.3 + Physics.Position.Y);
	glTexCoord2f(CurrentFrame / texture_frame + 1 / (double)texture_frame/*1/кол-во кадров (ширина кадра) */, 0.0); glVertex2f(0.3 + Physics.Position.X, 0.3 + Physics.Position.Y);
	glTexCoord2f(CurrentFrame / texture_frame + 1 / (double)texture_frame, 0.125); glVertex2f(0.3 + Physics.Position.X, -0.3 + Physics.Position.Y);
	glEnd(); // Конец обьекта рисуемого треугольниками

	glBindTexture(GL_TEXTURE_2D, Texture2); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(Physics.Position.X, Physics.Position.Y, 0);
	glRotated(-Physics.Angle * 180 / M_PI - 90, 0, 0, 1);
	glTranslated(-Physics.Position.X, -Physics.Position.Y, 0);
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.15 + Physics.Position.X, -0.15 + Physics.Position.Y);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.15 + Physics.Position.X, 0.15 + Physics.Position.Y);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.15 + Physics.Position.X, 0.15 + Physics.Position.Y);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.15 + Physics.Position.X, -0.15 + Physics.Position.Y);
	glEnd();
	glPopMatrix();
}

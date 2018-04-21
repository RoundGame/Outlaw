#include "character.h"
#include "core.h"

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
void Character::Animation()
{
	if (Physics.Acceleration.GetLength() != 0) //Если есть скорость то
	{	//Включаем анимацию передвижения
		CurrentFrame++;
		if (CurrentFrame > Legs.AnimationSize) //В анимации frame + 1 кадров, поэтому сбрасываем счетчик на 0, как только он перевалил за frame
			CurrentFrame = 0;
	}
	else // Иначе остановить персонажа
	{
		CurrentFrame = 0; //Текущий кадр - нулевой (стоим)
	}
}

void Character::Draw()
{
	glBindTexture(GL_TEXTURE_2D, Legs.Texture); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(Physics.Position.X, Physics.Position.Y, 0);
	glRotated(Direction * 180 / M_PI, 0, 0, 1);
	glTranslated(-Physics.Position.X, -Physics.Position.Y, 0);
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(CurrentFrame / Legs.AnimationSize, 0.125); glVertex2f(-0.2 + Physics.Position.X, -0.2 + Physics.Position.Y);
	glTexCoord2f(CurrentFrame / Legs.AnimationSize, 0.0); glVertex2f(-0.2 + Physics.Position.X, 0.2 + Physics.Position.Y);
	glTexCoord2f(CurrentFrame / Legs.AnimationSize + 1 / (double)Legs.AnimationSize, 0.0); glVertex2f(0.2 + Physics.Position.X, 0.2 + Physics.Position.Y);
	glTexCoord2f(CurrentFrame / Legs.AnimationSize + 1 / (double)Legs.AnimationSize, 0.125); glVertex2f(0.2 + Physics.Position.X, -0.2 + Physics.Position.Y);
	glEnd(); // Конец обьекта рисуемого треугольниками
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, Body.Texture); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(Physics.Position.X, Physics.Position.Y, 0);
	glRotated(Physics.Angle * 180 / M_PI, 0, 0, 1);
	glTranslated(-Physics.Position.X, -Physics.Position.Y, 0);
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.45 + Physics.Position.X, -0.45 + Physics.Position.Y);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.45 + Physics.Position.X, 0.45 + Physics.Position.Y);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.45 + Physics.Position.X, 0.45 + Physics.Position.Y);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.45 + Physics.Position.X, -0.45 + Physics.Position.Y);
	glEnd();
	glPopMatrix();
}
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
void Character::Animation()
{
	if (Move.Velocity.GetLength() > 0.01 || Move.Velocity.GetLength() < -0.01) //Если есть скорость то
	{	//Включаем анимацию передвижения
		CurrentFrame++;
		if (CurrentFrame > Leg.AnimationSize && CurrentFrame < 0) //В анимации frame + 1 кадров, поэтому сбрасываем счетчик на 0, как только он перевалил за frame
			CurrentFrame = 0;
	}
	else // Иначе остановить персонажа
	{
		CurrentFrame = 0; //Текущий кадр - нулевой (стоим)
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

//void Character::Update() 
//{
	//Move.Update(); // Изменяем позицию персонажа
//}

void Character::Draw()
{
	glBindTexture(GL_TEXTURE_2D, Leg.Texture); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(CurrentFrame / Leg.AnimationSize, (Direction - 1) / 8/*кол-во направлений*/ + 0.125); glVertex2f(-0.3 + Physics.Position.X, -0.3 + Physics.Position.Y);
	glTexCoord2f(CurrentFrame / Leg.AnimationSize, (Direction - 1) / 8/*кол-во направлений*/); glVertex2f(-0.3 + Physics.Position.X, 0.3 + Physics.Position.Y);
	glTexCoord2f(CurrentFrame / Leg.AnimationSize + 1 / (double)Leg.AnimationSize/*1/кол-во кадров (ширина кадра) */, (Direction - 1) / 8/*кол-во направлений*/); glVertex2f(0.3 + Physics.Position.X, 0.3 + Physics.Position.Y);
	glTexCoord2f(CurrentFrame / Leg.AnimationSize + 1 / (double)Leg.AnimationSize, (Direction - 1) / 8/*кол-во направлений*/ + 0.125); glVertex2f(0.3 + Physics.Position.X, -0.3 + Physics.Position.Y);
	glEnd(); // Конец обьекта рисуемого треугольниками

	glBindTexture(GL_TEXTURE_2D, Body.Texture); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(Move.Position.X, Move.Position.Y, 0);
	glRotated(-Move.Angle * 180 / M_PI - 90, 0, 0, 1);
	glTranslated(-Move.Position.X, -Move.Position.Y, 0);
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.15 + Physics.Position.X, -0.15 + Physics.Position.Y);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.15 + Physics.Position.X, 0.15 + Physics.Position.Y);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.15 + Physics.Position.X, 0.15 + Physics.Position.Y);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.15 + Physics.Position.X, -0.15 + Physics.Position.Y);
	glEnd();
	glPopMatrix();
}

/*void physical_component::update()
{
	acceleration = acceleration.getnormalize(); //нормализуем полученный вектор ускорения

	velocity.x += (acceleration.x * 0.01 - velocity.x * 0.1) * boost; //добавлем к вектору скорости вектор ускорения. boost - коэффициент ускорения
	velocity.y += (acceleration.y * 0.01 - velocity.y * 0.1) * boost; //вычитаем из вектора ускорения вектор текущей скорости, тем самым имитируем трение

	position.x += velocity.x * speed; //добавляем к вектору текущей позиции вектор текущей скорости
	position.y += velocity.y * speed;
}

void bullet::draw() // сейявс не используется
{
	if (isexist)
	{
		glbindtexture(gl_texture_2d, body.texture);
		glmatrixmode(gl_modelview);

		glpushmatrix();
		glloadidentity();
		gltranslated(move.position.x, move.position.y, 0);
		glrotated(-move.angle * 180 / m_pi - 90, 0, 0, 1);
		gltranslated(-move.position.x, -move.position.y, 0);

		glbegin(gl_quads);
			gltexcoord2f(0.0, 1.0); glvertex2f(-0.15 + move.position.x, -0.15 + move.position.y);
			gltexcoord2f(0.0, 0.0); glvertex2f(-0.15 + move.position.x, 0.15 + move.position.y);
			gltexcoord2f(1.0, 0.0); glvertex2f(0.15 + move.position.x, 0.15 + move.position.y);
			gltexcoord2f(1.0, 1.0); glvertex2f(0.15 + move.position.x, -0.15 + move.position.y);
		glend();
		glpopmatrix();
	}
}

void bullet::update() 
{

}*/
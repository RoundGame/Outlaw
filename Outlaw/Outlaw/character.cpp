#include "character.h"

void Character::Animation(int frame)
{
	if (CurrentAnimation)
	{
		CurrentFrame++;
		if (CurrentFrame > frame) //В анимации frame + 1 кадров, поэтому сбрасываем счетчик на 0, как только он перевалил за frame
			CurrentFrame = 0;
	}
}

void Character::Update() 
{
	float speed = 0.4;
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


#include "character.h"

void Character::Animation(int frame)
{
	if (CurrentAnimation)
	{
		CurrentFrame++;
		if (CurrentFrame > frame) //В анимации пять кадров, поэтому сбрасываем счетчик на 0, как только он перевалил за 4
			CurrentFrame = 0;
	}
}

void Character::Update() 
{
	//Vector velocity = Velocity.GetNormalize(); //Нормализуем вектор скорости
	if (Velocity.GetLength() > 0.01 || Velocity.GetLength() < -0.01) //Если есть скорость то
	{
		CurrentAnimation = true; //Включаем анимацию передвижения
		Position.X += Velocity.X;// * speed; //Добавляем к вектору игрока вектор его скорости
		Position.Y += Velocity.Y;// * speed;
		Velocity.X -= Velocity.X * 0.2;
		Velocity.Y -= Velocity.Y * 0.2;
	}
	else //Иначе
	{
		CurrentFrame = 0; //Текущий кадр - нулевой (стоим)
		CurrentAnimation = false; //Текущая анимация нулевая (стоим)
	}
}


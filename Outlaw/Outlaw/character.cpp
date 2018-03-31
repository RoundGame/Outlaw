#include "character.h"

void character::Animation(int frame)
{
	if (CurrentAnimation)
	{
		CurrentFrame++;
		if (CurrentFrame > frame) //В анимации пять кадров, поэтому сбрасываем счетчик на 0, как только он перевалил за 4
			CurrentFrame = 0;
	}
}

void character::Update() 
{
	Vector velocity = Velocity.GetNormalize(); //Нормализуем вектор скорости
	if (velocity.GetLength() != 0) //Если есть скорость то
	{
		CurrentAnimation = true; //Включаем анимацию передвижения
		Position.X += velocity.X * speed; //Добавляем к вектору игрока вектор его скорости
		Position.Y += velocity.Y * speed;
	}
	else //Иначе
	{
		CurrentFrame = 0; //Текущий кадр - нулевой (стоим)
		CurrentAnimation = false; //Текущая анимация нулевая (стоим)
	}
}


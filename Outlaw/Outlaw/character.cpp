#include "character.h"

void character::Animation() 
{
	if (CurrentAnimation == 1)
	{
		CurrentFrame++;
		if (CurrentFrame > 4) //В анимации пять кадров, поэтому сбрасываем счетчик на 0, как только он перевалил за 4
			CurrentFrame = 0;
	}
}

void character::Update() 
{
	Vector velocity = Velocity.GetNormalize(); //Нормализуем вектор скорости
	if (velocity.Len != 0) //Если есть скорость то
	{
		CurrentAnimation = 1; //Включаем анимацию передвижения
		Position.X += velocity.X * speed; //Добавляем к вектору игрока вектор его скорости
		Position.Y += velocity.Y * speed;
	}
	else //Иначе
	{
		CurrentFrame = 0; //Текущий кадр - нулевой (стоим)
		CurrentAnimation = 0; //Текущая анимация нулевая (стоим)
	}
}


#include "character.h"

void character::Update() {
	glutPostRedisplay(); //Обновляем экран
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
	glutTimerFunc(200, character::StaticUpdate, 0); // Задержка 20 мс перед новым вызовом функции
}
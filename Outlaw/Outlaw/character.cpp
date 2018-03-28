#include "character.h"

void character::Animation() 
{
	if (CurrentAnimation == 1)
	{
		CurrentFrame++;
		if (CurrentFrame > 4) //В анимации пять кадров, поэтому сбрасываем счетчик на 0, как только он перевалил за 4
			CurrentFrame = 0;
	}
	glutTimerFunc(100, character::StaticAnimation, 1); //Задержка 100 мс перед новым вызовом функции
};
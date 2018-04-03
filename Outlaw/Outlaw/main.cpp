#include "core.h"
using namespace std;

int main(int argc, char **argv)
{
	// Инициализация главного окна
	initGL(argc, argv);

	// Отрисовка
	glutDisplayFunc(Render);

	/*Цикл по подсчету координат перемещения персонажей и объектов
	timer_update - Частота обновления
	Update - Вызываемая функция
	value - Параметр значения обратного вызова */
	glutTimerFunc(timer_update, Update, 0);
	timer_animation = 10;
	/*Цикл по анимаций персонажей
	timer_update - Частота обновления
	Animation - Вызываемая функция
	value - Параметр значения обратного вызова */
	glutTimerFunc(timer_animation, Animation, 0);

	/* Регистрация изменение размеров окна. 
	Принимает функцию с параметрами: 
	w - ширина окна
	h - высота окна*/ 
	glutReshapeFunc(reshape_win_size);

	//Пробрасываем хук клавиатуры на текущий поток и записываем хендл хука в KeyboardHook
	Keyboard_Hook = SetWindowsHookExA(WH_KEYBOARD_LL, KeybdHookProc, NULL, 0);

	//waveOutReset(0);
	PlaySoundA("test.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	// Главный цикл
	glutMainLoop();
	return 0;
}
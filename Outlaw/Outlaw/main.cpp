#include "core.h"
using namespace std;
int main(int argc, char **argv)
{
	// Инициализация главного окна
	initGL(argc, argv);

	// Отрисовка
	glutDisplayFunc(render);

	/*Цикл по подсчету координат перемещения персонажей и объектов
	timer_update - Частота обновления
	Update - Вызываемая функция
	value - Параметр значения обратного вызова */
	glutTimerFunc(timer_update, Update, 0);

	/*Цикл по анимаций персонажей
	timer_update - Частота обновления
	Animation - Вызываемая функция
	value - Параметр значения обратного вызова */
	glutTimerFunc(timer_animation, Animation, 0);

	// Регистрация изменение размеров окна
	glutReshapeFunc(reshape_win_size);
	// Клавиатура
	glutKeyboardFunc(NormalKeys);
	glutKeyboardUpFunc(NormalKeysUp);
	glutSpecialFunc(SpecialKeys);

	// Главный цикл
	glutMainLoop();
	return 0;
}
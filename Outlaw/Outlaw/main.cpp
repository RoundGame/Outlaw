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
	// Клавиатура
	glutKeyboardFunc(NormalKeys); // Замечает нажитие клавиш печати (1-0, а-Я, a-Z)
	glutKeyboardUpFunc(NormalKeysUp); // Замечает отпускание клавиш печати
	glutSpecialFunc(SpecialKeys);	// Зачечает нажитие функциональных клавиш (F1-F12, tab ...) (эти клавиши не используют ascii)
	// Главный цикл
	glutMainLoop();
	return 0;
}
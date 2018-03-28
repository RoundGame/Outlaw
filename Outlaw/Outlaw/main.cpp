#include "core.h"

int main(int argc, char **argv)
{
	// Инициализация главного окна
	initGL(argc, argv);

	glutTimerFunc(100, character::StaticUpdateCB, 1);

	// Отрисовка
	glutDisplayFunc(render);
	// Цикл по подсчету координат
	glutTimerFunc(20, Update, 0);
	// Анимация
	glutTimerFunc(100, Animation, 0);
	// Регистрация обработанных данных
	// Изменение
	glutReshapeFunc(reshape_win_size);
	// Клавиатура
	glutKeyboardFunc(NormalKeys);
	glutKeyboardUpFunc(NormalKeysUp);
	glutSpecialFunc(SpecialKeys);
	// Основной цикл
	glutMainLoop();

	return 0;
}
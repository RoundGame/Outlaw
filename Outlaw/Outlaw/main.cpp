#include "core.h"

int main(int argc, char **argv)
{
	// Инициализация главного окна
	initGL(argc, argv);

	// Отрисовка
	glutDisplayFunc(render);

	// Регистрация обработанных данных
	glutReshapeFunc(reshape_win_size);

	// Основной цикл
	glutMainLoop();

	return 0;
}
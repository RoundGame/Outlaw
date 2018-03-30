#include "core.h"

int main(int argc, char **argv)
{
	// Инициализация главного окна
	initGL(argc, argv);

	// Отрисовка
	glutDisplayFunc(render);
	// Цикл по подсчету координат
	glutTimerFunc(20, Update, 0);
	// Анимация
	glutTimerFunc(100, Animation, 1); //Задержка 100 мс перед новым вызовом функции
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

// Вопросы
// 1. Как правильно считывать нажатие клавиш (не считывается при изменении языка или зажатом Shift)

﻿#include "core.h"
using namespace std;

int main(int argc, char **argv)
{
	// Инициализация GLUT
	initGL(argc, argv, true);

	// Отрисовка
	/*Единственный параметр этой функции - это указатель на функцию,
	которая будет отвечать за рисование в окне.*/
	glutDisplayFunc(Render);

	/* "glutTimerFunc", Функция устанавливает таймер.
	Первый параметр timer_update задает время в миллисекундах,
	по истечении которых вызывается функция, которая указана как второй параметр.
	Третий параметр value указывает индентификатор таймера.*/

	//	Update - Функция подсчета координат перемещения персонажей и объектов.
	glutTimerFunc(timer_update, Update, 0);

	// Animation - Функция анимации персонажей
	glutTimerFunc(timer_animation, Animation, 0);

	/* "glutReshapeFunc", Функция отслеживает изменения окна.
	Единственный параметр - это указатель на функцию, отвечающую за изменение 
	размеров окна, которая как видно должна принимать два параметра width и height,
	соответственно ширина и высота нового (измененного) окна.*/ 
	glutReshapeFunc(reshape_win_size);

	/*	HHOOK WINAPI SetWindowsHookEx(
	_In_ int       idHook,
	_In_ HOOKPROC  lpfn,
	_In_ HINSTANCE hMod,
	_In_ DWORD     dwThreadId	);
	Функция устанавливает определяемую программой процедуру
	фильтра (hook) в цепочку фильтров (hook).
	Первый параметр idHook - это тип процедуры захвата, которую нужно установить.
	Он принимает значение WH_KEYBOARD_LL(13) Соответственно устанавливает процедуру захвата, которая контролирует низкоуровневые события ввода клавиатуры.
	Второй параметр lpfn - это указатель на процедуру hook.
	Третий параметр hMod - это дескриптор DLL, содержащий процедуру hook, на которую указывает параметр lpfn. 
	Он принимает значение NULL, это говорит о том, что dwThreadId указывает поток, созданный текущим процессом, 
	и если процедура hook находится в коде, связанном с текущим процессом.
	Последний параметр DWORD - это идентификатор потока, с которым должна быть связана процедура hook.
	Для настольных приложений, если этот параметр равен 0, процедура hook связана со всеми существующими
	потоками, запущенными на том же рабочем столе, что и вызывающий поток. */
	Keyboard_Hook = SetWindowsHookExA(WH_KEYBOARD_LL, KeybdHookProc, NULL, 0);
	Keyboard_Hook = SetWindowsHookExA(WH_MOUSE_LL, MouseHookProc, NULL, 0);

	//Включаем фоновую музыку
	Play_Music();

	/*Начинает основной цикл GLUT обработки событий. В цикле событий обрабатываются все сообщения клавиатуры,
	мыши, таймера, перерисовывания и другие сообщения окна. Функция не возвращает ничего, пока программа не завершится.*/
	glutMainLoop();
	return 0;
}
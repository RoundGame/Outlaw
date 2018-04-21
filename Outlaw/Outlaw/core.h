#pragma once
#include <Windows.h>
#include "key.h"
#include "config.h"
#include <GL/glut.h>
#include "character.h"



static HHOOK Keyboard_Hook; // Хэндл хука клавиатуры
static HHOOK Mouse_Hook; // Хэндл хука мыши
static HWND Main_Window_Handle; // Хэндл главного окна, для последующего отключения обработчика нажатия клавиш

// Инициализация главного окна
void initGL(int argc, char **argv);
	// Регистрация изменения размеров окна
	void reshape_win_size(int w, int h);
	void SetFullScreen();
	// Сохранение и выход
	void Save(); 

// Отрисовка
void Render();
void Draw_Quad(Vector Position, Vector Size, Sprite Sprite); // Рисует квадрат в позиции Position и размера Size, где рисование объекта начинается с центра
void Animation(int Value); // Главная функция анимации
void Matrix_Rotate(Vector position, double angle); // Поворот в соответствии физическому компоненту

void Update(int Value);	// Главнвя функция изменения позиции
void CreateBullet();
bool Collision(Physical_component Physics1, Physical_component Physics2); // Возвращает истину, если Physics1 пересекает Physics2


LRESULT __stdcall KeybdHookProc(int code, WPARAM wParam, LPARAM lParam); // Для считываения нажатия клавиш 
LRESULT __stdcall MouseHookProc(int code, WPARAM wParam, LPARAM lParam); // Для считываения событий мыши
#pragma once
#include <Windows.h>
#include "key.h"
#include "config.h"
#include <GL/glut.h>
#include "character.h"

static HHOOK Keyboard_Hook; // Хэндл хука клавиатуры
static HWND Main_Window_Handle; // Хэндл главного окна, для последующего отключения обработчика нажатия клавиш

// Инициализация главного окна
void initGL(int argc, char **argv);

// Отрисовка
void Render();

// Регистрация изменения размеров окна
void reshape_win_size(int w, int h);
void SetFullScreen();

// Загрузка тексткуры texture1 - куда, name - путь к загружаемому файлу
void InitTexture(unsigned int& texture1, const char name[]);

void Animation(int Value); // Главная функция анимации
void Update(int Value);	// Главнвя функция изменения позиции

LRESULT __stdcall KeybdHookProc(int code, WPARAM wParam, LPARAM lParam); // Для считываения нажатия клавиш 
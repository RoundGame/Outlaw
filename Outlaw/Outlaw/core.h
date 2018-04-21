<<<<<<< HEAD
#pragma once

#include <GL/glut.h>
#include <soil.h>
#include <cstdio>
=======
﻿#pragma once
#include <Windows.h>
#include "key.h"
#include "config.h"
#include <GL/glut.h>
#include "character.h"
>>>>>>> develop

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
void Turn_to_goal(Physical_component Physics); // Поворот в соответствии физическому компоненту

void Update(int Value);	// Главнвя функция изменения позиции
void CreateBullet();

<<<<<<< HEAD
// �������� ��������� texture1 - ����, name - ���� � ������������ �����
void InitTexture(unsigned int& texture1, const char name[]);
=======
LRESULT __stdcall KeybdHookProc(int code, WPARAM wParam, LPARAM lParam); // Для считываения нажатия клавиш 
LRESULT __stdcall MouseHookProc(int code, WPARAM wParam, LPARAM lParam); // Для считываения событий мыши
>>>>>>> develop

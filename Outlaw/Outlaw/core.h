﻿#pragma once
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

// Генерация комнаты
void Generator_room(int Type, int Size, Vector Position);
void Entity_draw(Entity Entity);

// Отрисовка
void Render();

// Регистрация изменения размеров окна
void reshape_win_size(int w, int h);
void SetFullScreen();
void CreateBullet();

// Загрузка тексткуры texture1 - куда, name - путь к загружаемому файлу
void InitTexture(unsigned int& texture1, const char name[]);

void Animation(int Value); // Главная функция анимации
void Update(int Value);	// Главнвя функция изменения позиции
void Save(); // Сохранение и выход

LRESULT __stdcall KeybdHookProc(int code, WPARAM wParam, LPARAM lParam); // Для считываения нажатия клавиш 
LRESULT __stdcall MouseHookProc(int code, WPARAM wParam, LPARAM lParam); // Для считываения событий мыши
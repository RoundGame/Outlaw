#pragma once
#include <GL/glut.h>
#include "character.h"
#include "key.h"

// Инициализация главного окна
void initGL(int argc, char **argv);

// Отрисовка
void render();

// Регистрация изменения размеров окна
void reshape_win_size(int w, int h);

// Загрузка тексткуры texture1 - куда, name - путь к загружаемому файлу
void InitTexture(unsigned int& texture1, const char name[]);

void NormalKeysUp(unsigned char key, int x, int y);
void NormalKeys(unsigned char key, int x, int y);
void SpecialKeys(int key, int x, int y);

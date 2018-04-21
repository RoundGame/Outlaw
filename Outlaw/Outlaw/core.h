#pragma once

#include <GL/glut.h>
#include <soil.h>
#include <cstdio>

// ������������� �������� ����
void initGL(int argc, char **argv);

// ���������
void render();

// ����������� ��������� �������� ����
void reshape_win_size(int w, int h);

// �������� ��������� texture1 - ����, name - ���� � ������������ �����
void InitTexture(unsigned int& texture1, const char name[]);

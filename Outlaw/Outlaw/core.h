#pragma once
#include <GL/glut.h>

// ������������� �������� ����
void initGL(int argc, char **argv);

// ���������
void render();

// ����������� ��������� �������� ����
void reshape_win_size(int w, int h);

// �������� ��������� texture1 - ����, name - ���� � ������������ �����
void InitTexture(unsigned int& texture1, const char name[]);

void NormalKeys(unsigned char key, int x, int y);
void SpecialKeys(int key, int x, int y);

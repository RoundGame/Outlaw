#ifndef CORE_H
#define CORE_H

// ������������� �������� ����
void initGL(int argc, char **argv);

// ���������
void render();

// ����������� ��������� �������� ����
void reshape_win_size(int w, int h);

// �������� ��������� texture1 - ����, name - ���� � ������������ �����
void InitTexture(unsigned int& texture1, const char name[]);
#endif // !CORE_H

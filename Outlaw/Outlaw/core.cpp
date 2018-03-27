#include "core.h"
#include "key.h"
#include <soil.h>
#include <cstdio>

//��������
unsigned int Textures[6]; // ����������� ��������� ���-�� �������

						  // �������� ��������� texture1 - ����, name - ���� � ������������ �����
void InitTexture(unsigned int& texture1, const char name[])
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	int *width = new int(0),
		*height = new int(0);
	unsigned char* image = SOIL_load_image(name, width, height, 0, SOIL_LOAD_RGB);
	if (image == 0)
		printf("InitTexture ERROR : %s \n", name);

	glGenTextures(1, &texture1); // ��������� ��������

	// ��������� ����������
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// �������� ���������
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, (*width), (*height), GL_RGB, GL_UNSIGNED_BYTE, image);
	delete width;
	delete height;
}

// ������������� �������� ����
void initGL(int argc, char **argv)
{
	glutInit(&argc, argv);	// ������������� glut
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); 	// ��������� ���������� ��������� ���
																// GLUT_DEPTH - ���������� �������
																// GLUT_DOUBLE - ����� ������� �����������
																// GLUT_RGBA - �������� �����(RGB) + ����� �����(�)
	glutInitWindowSize(800, 600);	 // ������ ������ � ��������
	glutInitWindowPosition(100, 100); // ������� ���� ������������ ������ �������� ����(0,0) � ��������
	glutCreateWindow("Roggame");	 // ��� ����

									 // ������������� �������
	InitTexture(Textures[0], "test.jpg");
	InitTexture(Textures[1], "test.bmp");
	InitTexture(Textures[2], "a3.jpg");
	//InitTexture(Textures[3], "b.bmp");
	//InitTexture(Textures[4], "c.png");

	//printf("InitGL - complete\n"); // ����� ������ �������� �������� ����
}

// ���������
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ������� ������� � �����
	glClearColor(0.2, 0.2, 0.5, 1);
	glEnable(GL_TEXTURE_2D); // �������� ���������� ���������������
	glBindTexture(GL_TEXTURE_2D, Textures[0]); // ����������� ��������, ����� ����� �������������� ���, �� ����� ��������
	glBegin(GL_TRIANGLE_FAN); // ������ ������� ��������� ��������������
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, 0.7);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, 0.7);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, 0.7);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, 0.7);
	glEnd(); // ����� ������� ��������� ��������������

	glBindTexture(GL_TEXTURE_2D, Textures[1]); // ����������� ��������, ����� ����� �������������� ���, �� ����� ��������
	glBegin(GL_TRIANGLE_FAN); // ������ ������� ��������� ��������������
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.25, -0.25);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.25, 0.25);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.25, 0.25);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.25, -0.25);
	glEnd(); // ����� ������� ��������� ��������������
	glutSwapBuffers(); // ������ ������ �� ����� ������������ 
}

// ����������� ��������� �������� ����
void reshape_win_size(int w, int h)
{
	// ���������� ���� ���������
	glViewport(0, 0, w, h);
	printf("w - %d, h - %d \n", w, h); // ����� �������� ������� ���� � �������
}

void NormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case KEY_ESC: exit(0); break;
	}
		printf("key : %d \n", key);
}

void SpecialKeys(int key, int x, int y)
{

}
#include "core.h"

int main(int argc, char **argv)
{
	// ������������� �������� ����
	initGL(argc, argv);

	// ���������
	glutDisplayFunc(render);

	// ����������� ������������ ������
	glutReshapeFunc(reshape_win_size);

	// �������� ����
	glutMainLoop();

	return 0;
}
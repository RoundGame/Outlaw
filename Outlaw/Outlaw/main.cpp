#include "core.h"

int main(int argc, char **argv)
{
	// ������������� �������� ����
	initGL(argc, argv);

	// ���������
	glutDisplayFunc(render);

	// ����������� ������������ ������
	//���������
	glutReshapeFunc(reshape_win_size);
	// ����������
	glutKeyboardFunc(NormalKeys);
	glutSpecialFunc(SpecialKeys);
	// �������� ����
	glutMainLoop();

	return 0;
}
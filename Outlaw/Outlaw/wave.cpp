#include "wave.h"

Wave::Wave(char * filename) 
{
	HInstance = GetModuleHandle(0);

	// �������� �����
	std::ifstream infile(filename, std::ios::binary);
	if (!infile)
	{
		printf("Wave load file error:%c", filename);
		return;
	}

	infile.seekg(0,std::ios::end); // ��������� ����� �����
	int length = infile.tellg();
	buffer = new char[length]; // ������� ������
	infile.seekg(0, std::ios::beg); // ��������� ����� �� ������ �����
	infile.read(buffer, length); // ������� ����

	infile.close();
	success = true;
};

Wave::Wave() 
{
	PlaySound(NULL, 0, 0); // ���������� ������������ ����� ������
	delete[] buffer;      // �������� �����
};

void Wave::play(bool async) 
{
	if (!success)
		return;
	if (async)
		PlaySoundA(buffer, HInstance, SND_MEMORY | SND_ASYNC);
	else
		PlaySoundA(buffer, HInstance, SND_MEMORY);
};

bool Wave::issuccess() 
{
	return success;
};
#include "wave.h"

Wave::Wave(char * filename) 
{
	HInstance = GetModuleHandle(0);

	// Загрузка файла
	std::ifstream infile(filename, std::ios::binary);
	if (!infile)
	{
		printf("Wave load file error:%c", filename);
		return;
	}

	infile.seekg(0,std::ios::end); // Посчитаем длину файла
	int length = infile.tellg();
	buffer = new char[length]; // Выделим память
	infile.seekg(0, std::ios::beg); // Установим выход на начало файла
	infile.read(buffer, length); // Прочтем файл

	infile.close();
	success = true;
};

Wave::Wave() 
{
	PlaySound(NULL, 0, 0); // Остановить проигрывание любой музыки
	delete[] buffer;      // очистить буфер
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
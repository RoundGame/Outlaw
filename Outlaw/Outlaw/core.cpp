#include "core.h"
#include <soil.h>
#include <cstdio>
#include <iostream>
using namespace std;
//текстура
unsigned int Textures[6]; // Максимально доступное кол-во текстур

Character Player;
int volume;

void Update(int Value) 
{
	Player.Acceleration.X = -1 * key[LEFT].isPressed + key[RIGHT].isPressed; 
	Player.Acceleration.Y = -1 * key[DOWN].isPressed + key[UP].isPressed;
	Player.Acceleration = Player.Acceleration.GetNormalize(); //Нормализуем полученный вектор ускорения

	Player.Update(); // Изменение позиции игрока
	glutPostRedisplay(); // Обновляем экран
	glutTimerFunc(timer_update, Update, 0); // Задержка 20 мс перед новым вызовом функции
}
void Animation(int Value)
{
	Player.Animation(4); // Анимация игрока
	glutTimerFunc(timer_animation, Animation, 1); //Задержка 100 мс перед новым вызовом функции
};

// Загрузка тексткуры texture1 - куда, name - путь к загружаемому файлу
void InitTexture(unsigned int& texture1, const char name[])
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	int width, height, channels;
	unsigned char* image = SOIL_load_image(name, &width, &height, &channels, SOIL_LOAD_RGBA);
	if (image == 0)
		printf("InitTexture ERROR : %s \n", name);

	glGenTextures(1, &texture1); // Генерация текстуры

	// Установка параметров
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Создание миникарты
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

// Инициализация главного окна
void initGL(int argc, char **argv)
{
	glutInit(&argc, argv);	// Инициализация glut
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); 	// Установка парометров отрисовки где
																// GLUT_DEPTH - разрешение глубины
																// GLUT_DOUBLE - режим двойной буферизации
																// GLUT_RGBA - цветовой канал(RGB) + альфа канал(А)
	glutInitWindowSize(800, 600);	 // Размер экрана в пикселях
	glutInitWindowPosition(100, 100); // Позиция окна относительно левого верхнего угла(0,0) в пикселях
	glutCreateWindow("Roggame");	 // Имя окна

									 // Инициализация текстур
	InitTexture(Textures[0], "latest.png");
	InitTexture(Textures[1], "test.png");
	//InitTexture(Textures[2], "a3.jpg");
	//InitTexture(Textures[3], "b.bmp");
	//InitTexture(Textures[4], "c.png");

	//printf("InitGL - complete\n"); // Нужно сделаь проверку создания окна
	//Биндим клавиши
	key[LEFT].Nominal = KEY_A;
	key[RIGHT].Nominal = KEY_D;
	key[UP].Nominal = KEY_W;
	key[DOWN].Nominal = KEY_S;
	waveOutGetVolume(0, (LPDWORD)&volume);
}

// Отрисовка
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка буферов глубины и цвета
	glClearColor(0.2, 0.2, 0.5, 1);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);
	glEnable(GL_TEXTURE_2D); // Включает двухмерное текстурирование

	glBindTexture(GL_TEXTURE_2D, Textures[0]); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(0.0, 1.0); glVertex2f(-0.3 + Player.Position.X, -0.3 + Player.Position.Y);
	glTexCoord2f(0.0, 0.0); glVertex2f(-0.3 + Player.Position.X, 0.3 + Player.Position.Y);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.3 + Player.Position.X, 0.3 + Player.Position.Y);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.3 + Player.Position.X, -0.3 + Player.Position.Y);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, Textures[1]); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(Player.CurrentFrame / 5, 0.5); glVertex2f(-0.25 + Player.Position.X, -0.25 + Player.Position.Y);
	glTexCoord2f(Player.CurrentFrame / 5, 0.0); glVertex2f(-0.25 + Player.Position.X, 0.25 + Player.Position.Y);
	glTexCoord2f(Player.CurrentFrame / 5 + 0.2, 0.0); glVertex2f(0.25 + Player.Position.X, 0.25 + Player.Position.Y);
	glTexCoord2f(Player.CurrentFrame / 5 + 0.2, 0.5); glVertex2f(0.25 + Player.Position.X, -0.25 + Player.Position.Y);
	glEnd(); // Конец обьекта рисуемого треугольниками

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glutSwapBuffers(); // Замена буфера на вновь отрисованный 
}

// Регистрация изменения размеров окна
void reshape_win_size(int w, int h)
{
	// Определяем окно просмотра
	glViewport(0, 0, w, h);
	printf("w - %d, h - %d \n", w, h); // вывод текущего размера окна в консоль
}

/* Состояние прилржения 
   false - окно			
   true - полный экран */
bool IsFullScreen = false;

void SetFullScreen() //Функция установки полного экрана или возвращения в окно
{
	if (!IsFullScreen)
	{
		glutFullScreen();	// Запуск полноэкранного режима
		IsFullScreen = !IsFullScreen;
	}
	else
	{
		glutReshapeWindow(800, 600);	 // Установка размеров окна в 800х600
		glutPositionWindow(GetSystemMetrics(SM_CXSCREEN) / 2 - 400, GetSystemMetrics(SM_CYSCREEN) / 2 - 300);	// Перемещение окна в центр экрана
		IsFullScreen = !IsFullScreen;
	}
}

//Функция, которая вызывается при изменении состоянии клавиатуры
//Если code < 0, то нужно пробросить хук дальше, wParam хранит данные о том, нажата клавиша или отпущена, lParam хранит указатель на структуру KBDLLHOOKSTRUCT
LRESULT __stdcall KeybdHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	//Данная структура хранит информацию о клавише
	KBDLLHOOKSTRUCT *KEY = (KBDLLHOOKSTRUCT*)lParam; //Получаем указатель на структуру данных о нажатой клавише

	if (code >= 0) //Если нет ошибок и событие вызвано клавиатурой
	{
		for (int i = 0; i < key_length; i++)
		{
			if (key[i].Nominal == KEY->vkCode)
			{
				if (wParam == WM_KEYDOWN)
					key[i].isPressed = true;
				else
					key[i].isPressed = false;
			}
		}
		if (KEY->vkCode == KEY_R)
		{
			volume += 134219776;
			waveOutSetVolume(0, volume);
		}
		if (KEY->vkCode == KEY_E)
		{
			volume -= 134219776;
			waveOutSetVolume(0, volume);
		}
		if (KEY->vkCode == KEY_C && wParam == WM_KEYUP)
		{
			if (Player.boost == 4)
				Player.boost = 0.1;
			else
				Player.boost = 4;
		}
	}
	return CallNextHookEx(Keyboard_Hook, code, wParam, lParam); //Пробрасываем хук дальше
}

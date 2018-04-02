#include "core.h"
#include <soil.h>
#include <cstdio>
#include <iostream>
using namespace std;
//текстура
unsigned int Textures[6]; // Максимально доступное кол-во текстур

character player;
HHOOK extern KeyboardHook; //Хэндл хука клавиатуры

void Update(int Value) {
	player.Update(); // Изменение позиции игрока
	glutPostRedisplay(); // Обновляем экран
	glutTimerFunc(timer_update, Update, 0); // Задержка 20 мс перед новым вызовом функции
}
void Animation(int Value)
{
	player.Animation(4); // Анимация игрока
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
	InitTexture(Textures[0], "test.jpg");
	InitTexture(Textures[1], "test.png");
	//InitTexture(Textures[2], "a3.jpg");
	//InitTexture(Textures[3], "b.bmp");
	//InitTexture(Textures[4], "c.png");

	//printf("InitGL - complete\n"); // Нужно сделаь проверку создания окна
}

// Отрисовка
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка буферов глубины и цвета
	glClearColor(0.2, 0.2, 0.5, 1);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);
	glEnable(GL_TEXTURE_2D); // Включает двухмерное текстурирование

	glBindTexture(GL_TEXTURE_2D, Textures[1]); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(player.CurrentFrame / 5, 0.5); glVertex2f(-0.25 + player.Position.X, -0.25 + player.Position.Y);
	glTexCoord2f(player.CurrentFrame / 5, 0.0); glVertex2f(-0.25 + player.Position.X, 0.25 + player.Position.Y);
	glTexCoord2f(player.CurrentFrame / 5 + 0.2, 0.0); glVertex2f(0.25 + player.Position.X, 0.25 + player.Position.Y);
	glTexCoord2f(player.CurrentFrame / 5 + 0.2, 0.5); glVertex2f(0.25 + player.Position.X, -0.25 + player.Position.Y);
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
bool fullScreen = false;	

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{


	case GLUT_KEY_F1: // Включает и отключает полноэкранный режим
		if (!fullScreen)
		{
			glutFullScreen();	// Запуск полноэкранного режима
			fullScreen = !fullScreen;
		}
		else 
		{
			glutReshapeWindow(800, 600);	 // Установка размеров окна в 800х600
			glutPositionWindow(0, 0);	// Перемещение окна в левый верхний угол
			fullScreen = !fullScreen;
		}
		cout << key << '\n';
		break;
	default:
		cout << key << '\n';
		break;
	}
}

LRESULT __stdcall HookProc(int code, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT *KEY = (KBDLLHOOKSTRUCT*)lParam; //Получаем указатель на структуру данных о нажатой клавише

	if (code >= 0) //Если нет ошибок и событие вызвано клавиатурой
	{
		if (wParam == WM_KEYDOWN) //Если клавиша нажата
		{
			switch (KEY->vkCode)
			{
			case VK_ESCAPE: exit(0);
				break;
			case KEY_A: //Если нажата клавиша A, то вектор скорости по X ставим равным -1, так как двигаемся налево
				player.Velocity.X = -1;
				break;
			case KEY_D: //Если нажата клавиша D, то вектор скорости по X ставим равным 1, так как двигаемся направо
				player.Velocity.X = 1;
				break;
			case KEY_W: //Если нажата клавиша W, то вектор скорости по Y ставим равным 1, так как двигаемся вверх
				player.Velocity.Y = 1;
				break;
			case KEY_S: //Если нажата клавиша S, то вектор скорости по X ставим равным -1, так как двигаемся вниз
				player.Velocity.Y = -1;
				break;
			default:
				cout << KEY->vkCode << '\n';
				break;
			}
		}
		else if (wParam == WM_KEYUP) //Если клавиша отпущена
		{
			switch (KEY->vkCode)
			{
			case KEY_A: //Если отпущена клавиша A, то останавливаемся на оси X, так как двигались налево
				player.Velocity.X = 0;
				break;
			case KEY_D: //Если отпущена клавиша D, то останавливаемся на оси X, так как двигались направо
				player.Velocity.X = 0;
				break;
			case KEY_W: //Если отпущена клавиша W, то останавливаемся на оси Y, так как двигались вверх
				player.Velocity.Y = 0;
				break;
			case KEY_S: //Если отпущена клавиша S, то останавливаемся на оси Y, так как двигались вниз
				player.Velocity.Y = 0;
				break;
			default:
				break;
			}
		}
	}
	return CallNextHookEx(KeyboardHook, code, wParam, lParam);
}

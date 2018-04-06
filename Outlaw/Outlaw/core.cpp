#include "core.h"
#include <soil.h>
#include <cstdio>
#include <iostream>
using namespace std;

Character Player; // Создаем игрока
int volume; // Тестовая переменная громкости звука

/*Цикл по подсчету координат перемещения персонажей и объектов */
void Update(int Value) 
{
	// Высчитывание перемещения игрока
	Player.Acceleration.X = -1 * key[LEFT].isPressed + key[RIGHT].isPressed; // Получаем направление движения по X
	Player.Acceleration.Y = -1 * key[DOWN].isPressed + key[UP].isPressed;	// Получаем направление движения по Y

	Player.Acceleration = Player.Acceleration.GetNormalize(); //Нормализуем полученный вектор ускорения
	if (Player.Acceleration.GetLength() != 0) //Если длина вектора равна нулю, то мы стоим и не нужно считать новое направление
	{
		if (Player.Acceleration.Y >= 0) // Считаем направление персонажа при движении (u - вперед, l - влево, r - право) u, r, l, ur, ul
			Player.Direction = round(-2 * Player.Acceleration.X + 3);
		else	// Считаем направление персонажа при движении (d - вниз, l - влево, r - право) d, dr, dl
			Player.Direction = round(2 * Player.Acceleration.X + 7);
		// Записываем получившееся число в Direction
	}

	Player.Update(); // Изменение позиции игрока

	glutPostRedisplay(); // Обновляем экран
	glutTimerFunc(timer_update, Update, 0); // Задержка 20 мс перед новым вызовом функции
}

//Функция анимации персонажей
void Animation(int Value)
{
	Player.Animation(7); // Анимация игрока, принемаемый параметр количество кадров анмайии
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
	glutCreateWindow("Outlaw");	 // Имя окна

	Main_Window_Handle = GetActiveWindow(); // Запоминаем главное окно, что бы в последстыии отключать обработчик клавиш если оно свернуто.

	// Инициализация текстур
	InitTexture(Player.Texture, "Character.png");

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
	glAlphaFunc(GL_GREATER, 0.5f); // Порог прорисовки прозрачности
	glEnable(GL_TEXTURE_2D); // Включает двухмерное текстурирование

	Player.Draw(); // Рисуем игрока

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
int Window_X, Window_Y, Window_Width, Window_Height; //Данные окна

void SetFullScreen() //Функция установки полного экрана или возвращения в окно
{
	if (!IsFullScreen)
	{
		RECT rect = RECT(); //Прямоугольник
		/*Извлекает размеры ограничивающего прямоугольника указанного окна.
		Размеры указаны в координатах экрана, которые относятся к верхнему левому углу экрана.*/
		GetWindowRect(GetActiveWindow(), &rect); //Записываем прямоугольник окна в rect
		Window_X = rect.left; //Координаты левого верхнего угла
		Window_Y = rect.top;
		Window_Width = rect.right - rect.left; //Координаты правого нижнего минус координаты левого верхнего равно размеры окна
		Window_Height = rect.bottom - rect.top;
		glutFullScreen();	// Запуск полноэкранного режима
		IsFullScreen = !IsFullScreen;
	}
	else
	{
		glutReshapeWindow(Window_Width, Window_Height);	 // Установка первоначальных размеров окна
		glutPositionWindow(Window_X, Window_Y);	// Перемещение окна в первоначальное положение
		IsFullScreen = !IsFullScreen;
	}
}

//Функция, которая вызывается при изменении состоянии клавиатуры
//Если code < 0, то нужно пробросить хук дальше, wParam хранит данные о том, нажата клавиша или отпущена, lParam хранит указатель на структуру KBDLLHOOKSTRUCT
LRESULT __stdcall KeybdHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	//Данная структура хранит информацию о клавише
	KBDLLHOOKSTRUCT *KEY = (KBDLLHOOKSTRUCT*)lParam; //Получаем указатель на структуру данных о нажатой клавише

	if (code >= 0 && GetActiveWindow() == Main_Window_Handle) //Если нет ошибок и событие вызвано клавиатурой и если хэндл активного окна совпадает с хэндлом нашего окна, то активно наше оконо и можно обрабатывать нажатия
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
		if (KEY->vkCode == VK_F11 && wParam == WM_KEYUP)
			SetFullScreen();
	}
	return CallNextHookEx(Keyboard_Hook, code, wParam, lParam); //Пробрасываем хук дальше
}

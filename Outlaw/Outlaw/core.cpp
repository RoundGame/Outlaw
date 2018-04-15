﻿#include "core.h"
#include <soil.h>
#include <cstdio>



Entity entity; // тестовый блок
const int bullet_count = 25;
Object bullet[bullet_count];
Character Player; // Создаем игрока
int volume; // Тестовая переменная громкости звука
POINT Cross;
double bullet_Speed = 0.2;

struct window
{
	//	позиция_х позиция_у
	Vector Position;
	// X ширина_окна	 Y высота_окна
	Vector size;
} window;

/*Цикл по подсчету координат перемещения персонажей и объектов */
void Update(int Value) 
{
	RECT rect = RECT(); //Прямоугольник
						/*Извлекает размеры ограничивающего прямоугольника указанного окна.
						Размеры указаны в координатах экрана, которые относятся к верхнему левому углу экрана.*/
	GetWindowRect(GetActiveWindow(), &rect); //Записываем прямоугольник окна в rect
	window.Position.X = rect.left; //Координаты левого верхнего угла
	window.Position.Y = rect.top;
	window.size.X = rect.right - rect.left; //Координаты правого нижнего минус координаты левого верхнего равно размеры окна
	window.size.Y = rect.bottom - rect.top;

	// Высчитывание перемещения игрока
	Player.Physics.Acceleration.X = -1 * key[LEFT].isPressed + key[RIGHT].isPressed; // Получаем направление движения по X
	Player.Physics.Acceleration.Y = -1 * key[DOWN].isPressed + key[UP].isPressed;	// Получаем направление движения по Y

	Player.Update(); // Изменение позиции игрока

	for (int i = 0; i < bullet_count; i++)
	{
		if (bullet[i].isExist)
		{
			bullet[i].Physics.Update(false);
			if (bullet[i].Physics.Position.X >= 2.0 || bullet[i].Physics.Position.X <= -2.0 || bullet[i].Physics.Position.Y >= 2.0 || bullet[i].Physics.Position.Y <= -2.0)
				bullet[i].isExist = false;
		}
	}

	Vector way = Vector(Cross.x - window.Position.X - (Player.Physics.Position.X + 1.0) * window.size.X / 2, Cross.y - window.Position.Y + (Player.Physics.Position.Y - 1.0) * window.size.Y / 2);
	way = way.GetNormalize();
	if (way.Y >= 0)
		Player.Physics.Angle = acos(way.X);
	else
		Player.Physics.Angle = -acos(way.X);

	glutPostRedisplay(); // Обновляем экран
	glutTimerFunc(timer_update, Update, Value); // Задержка 20 мс перед новым вызовом функции
}

// Сохранение и выход
void Save()
{
	printf("saving\n");
	exit(0);
};

//Функция анимации персонажей
void Animation(int Value)
{
	Player.Animation(); // Анимация игрока, принемаемый параметр количество кадров анмайии
	glutTimerFunc(timer_animation, Animation, Value); //Задержка 100 мс перед новым вызовом функции
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
	InitTexture(entity.Texture, "cobblestone.png");
	Player.Leg.Load("Character.png");
	Player.Body.Load("test.jpg");
	for (int i = 0; i < bullet_count; i++)
	{

		bullet[i].Body.Load("bullet.png");
		bullet[i].Physics.Position.Y = 2.0;
	}

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
	glClearColor(0, 0, 0, 1); // Устанавливаем цвет фона

	// Фон
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2f(-1, -1);
		glTexCoord2f(1, 1); glVertex2f(1, -1);
		glTexCoord2f(1, 0); glVertex2f(1, 1);
		glTexCoord2f(0, 0); glVertex2f(-1, 1);
	glEnd();

	glEnable(GL_ALPHA_TEST);	// Рразрешаем использовать прозрвачные текстуры
	glAlphaFunc(GL_GREATER, 0.5f); // Порог прорисовки прозрачности
	glEnable(GL_TEXTURE_2D); // Включает двухмерное текстурирование

	Player.Draw(); // Рисуем игрока

	for (int i = 0; i < bullet_count; i++)
	{
		if (bullet[i].isExist)
		{
			glBindTexture(GL_TEXTURE_2D, bullet[i].Body.Texture);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glTranslated(bullet[i].Physics.Position.X, bullet[i].Physics.Position.Y, 0);
			glRotated(-bullet[i].Physics.Angle * 180 / M_PI - 90, 0, 0, 1);
			glTranslated(-bullet[i].Physics.Position.X, -bullet[i].Physics.Position.Y, 0);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0); glVertex2f(-0.15 + bullet[i].Physics.Position.X, -0.15 + bullet[i].Physics.Position.Y);
			glTexCoord2f(0.0, 0.0); glVertex2f(-0.15 + bullet[i].Physics.Position.X, 0.15 + bullet[i].Physics.Position.Y);
			glTexCoord2f(1.0, 0.0); glVertex2f(0.15 + bullet[i].Physics.Position.X, 0.15 + bullet[i].Physics.Position.Y);
			glTexCoord2f(1.0, 1.0); glVertex2f(0.15 + bullet[i].Physics.Position.X, -0.15 + bullet[i].Physics.Position.Y);
			glEnd();
			glPopMatrix();
		}
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glutSwapBuffers(); // Замена буфера на вновь отрисованный 
}

// Генерация комнаты
void Generator_room(int Type, int Size, Vector Position)
{
	printf("Generator_room\n");

}

void Entity_draw(Entity Entity)
{
	glBindTexture(GL_TEXTURE_2D, Entity.Texture);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f((1 - Entity.Size) - Entity.Position.X * 2, (1 - Entity.Size) - Entity.Position.Y * 2);
	glTexCoord2f(1, 1); glVertex2f((1 + Entity.Size) - Entity.Position.X * 2, (1 - Entity.Size) - Entity.Position.Y * 2);
	glTexCoord2f(1, 0); glVertex2f((1 + Entity.Size) - Entity.Position.X * 2, (1 + Entity.Size) - Entity.Position.Y * 2);
	glTexCoord2f(0, 0);	glVertex2f((1 - Entity.Size) - Entity.Position.X * 2, (1 + Entity.Size) - Entity.Position.Y * 2);
	glEnd();
}

// Регистрация изменения размеров окна
void reshape_win_size(int w, int h)
{
	// Определяем окно просмотра
	if (w * win_heigh > h * win_width)
		glViewport((w - h * win_width / win_heigh) / 2, 0, h * win_width / win_heigh, h);
	else
		glViewport(0, (h - w * win_heigh / win_width) / 2, w, w * win_heigh / win_width); // Функци устанавливает область отрисовки внутри окна
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
		glutReshapeWindow(window.size.X, window.size.Y);	 // Установка первоначальных размеров окна
		glutPositionWindow(window.Position.X, window.Position.Y);	// Перемещение окна в первоначальное положение
		IsFullScreen = !IsFullScreen;
	}
}

//Функция, которая вызывается при изменении состоянии клавиатуры
//Если code < 0, то нужно пробросить хук дальше, wParam хранит данные о том, нажата клавиша или отпущена, lParam хранит указатель на структуру KBDLLHOOKSTRUCT
LRESULT __stdcall KeybdHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0 && GetActiveWindow() == Main_Window_Handle) //Если нет ошибок и событие вызвано клавиатурой и если хэндл активного окна совпадает с хэндлом нашего окна, то активно наше оконо и можно обрабатывать нажатия
	{
		//Данная структура хранит информацию о клавише
		KBDLLHOOKSTRUCT *KEY = (KBDLLHOOKSTRUCT*)lParam; //Получаем указатель на структуру данных о нажатой клавише

		for (int i = 0; i < sizeof(gamekey); i++) // Проверяем игровые клавиши на нажитие, где sizeof(gamekey) - доступное кол-во клавиш
		{
			if (key[i].Nominal == KEY->vkCode)
			{
				if (wParam == WM_KEYDOWN) // Изменяем их состояние на соответственные значения 
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
			if (Player.Physics.Boost == 4)
				Player.Physics.Boost = 0.1;
			else
				Player.Physics.Boost = 4;
		}
		if (KEY->vkCode == VK_F11 && wParam == WM_KEYUP)
			SetFullScreen();
		if (KEY->vkCode == VK_ESCAPE)
			Save();

	}
	return CallNextHookEx(Keyboard_Hook, code, wParam, lParam); //Пробрасываем хук дальше
}

LRESULT __stdcall MouseHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0 && GetActiveWindow() == Main_Window_Handle) //Если нет ошибок и событие вызвано мышью и если хэндл активного окна совпадает с хэндлом нашего окна, то активно наше оконо и можно обрабатывать события мыши
	{
		MSLLHOOKSTRUCT *MOUSE = (MSLLHOOKSTRUCT*)lParam;

		if (wParam == WM_MOUSEMOVE)
		{
			Cross = MOUSE->pt;
		}
		if (wParam == WM_LBUTTONDOWN)
		{
			int k = -1;
			for (int i = 0; i < bullet_count && k == -1; i++)
			{
				if (!bullet[i].isExist)
				{
					bullet[i].isExist = true;
					k = i;
				}
			}
			if (k == -1) 
				k = 0;
			bullet[k].Physics.Position.X = Player.Physics.Position.X;
			bullet[k].Physics.Position.Y = Player.Physics.Position.Y;
			bullet[k].Physics.Angle = Player.Physics.Angle;
			bullet[k].Physics.Velocity.X = sin(bullet[k].Physics.Angle + M_PI / 2) * bullet_Speed;
			bullet[k].Physics.Velocity.Y = cos(bullet[k].Physics.Angle + M_PI / 2) * bullet_Speed;
		}
		if (wParam == WM_RBUTTONDOWN)
		{
			if (bullet_Speed == 0.3)
				bullet_Speed = 0.03;
			else 
				bullet_Speed = 0.3;
		}
	}
	return CallNextHookEx(Keyboard_Hook, code, wParam, lParam); //Пробрасываем хук дальше
}
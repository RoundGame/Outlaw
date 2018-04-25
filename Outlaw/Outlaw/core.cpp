#include "core.h"
#include <soil.h>
#include <cstdio>

struct Window
{
	//	позиция_х позиция_у
	Vector Position;
	// X ширина_окна	 Y высота_окна
	Vector Size;
	Vector Render_Position; // Позиция отрисовываемой области
	Vector Render_Size; // Размер отрисовываемой области
} Window;

Entity entity; // тестовый блок
const int bullet_count = 30;
const int wall_count = 56;
Object bullet[bullet_count];
Object Cross;
Character Player; // Создаем игрока
Static_Object Wall[wall_count];
int volume; // Тестовая переменная громкости звука
int BurstMode = 0; //Режим стрельбы (0 - одиночными, 2 - очередью)
bool isMousePressed = false;

/*Цикл по подсчету координат перемещения персонажей и объектов */
void Update(int Value) 
{
	RECT rect = RECT(); //Прямоугольник
						/*Извлекает размеры ограничивающего прямоугольника указанного окна.
						Размеры указаны в координатах экрана, которые относятся к верхнему левому углу экрана.*/
	GetWindowRect(GetActiveWindow(), &rect); //Записываем прямоугольник окна в rect
	Window.Position.X = rect.left; //Координаты левого верхнего угла
	Window.Position.Y = rect.top;
	Window.Size.X = rect.right - rect.left; //Координаты правого нижнего минус координаты левого верхнего равно размеры окна
	Window.Size.Y = rect.bottom - rect.top;

	// Высчитывание перемещения игрока //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Player.Physics.Acceleration.X = -1 * key[LEFT].isPressed + key[RIGHT].isPressed; // Получаем направление движения по X
	Player.Physics.Acceleration.Y = -1 * key[DOWN].isPressed + key[UP].isPressed;	// Получаем направление движения по Y
	Player.Physics.Acceleration = Player.Physics.Acceleration.GetNormalize();
	Vector Velocity = Player.Physics.Velocity.GetNormalize();
	if (Player.Physics.Acceleration.GetLength() != 0)
	{
		if (Velocity.Y >= 0)
			Player.Direction = acos(Velocity.X);
		else
			Player.Direction = -acos(Velocity.X);
	}

	Vector Wall_Resistance;
	for (int i = 0; i < wall_count; i++)
	{
		if (Collision(Player.Physics.Position, Player.Legs.Size, Wall[i].Position, Wall[i].Body.Size))
		{
			Wall_Resistance.X += Player.Physics.Position.X - Wall[i].Position.X;
			Wall_Resistance.Y += Player.Physics.Position.Y - Wall[i].Position.Y;
		}
	}
	Wall_Resistance = Wall_Resistance.GetNormalize();
	Wall_Resistance.X = (int)(Wall_Resistance.X * 2 / sqrt(2));
	Wall_Resistance.Y = (int)(Wall_Resistance.Y * 2 / sqrt(2));
	Player.Physics.Acceleration.X += Wall_Resistance.X;
	Player.Physics.Acceleration.Y += Wall_Resistance.Y;

	//Player.Target_TO(Cross.Physics.Position); // Настроить 
	Vector way = Vector(Cross.Physics.Position.X - Player.Physics.Position.X, Cross.Physics.Position.Y - Player.Physics.Position.Y);
	way.X *= (double)win_height / win_width * Window.Render_Size.X / 2;
	way.Y *= Window.Render_Size.Y / 2;
	way = way.GetNormalize();

	if (way.Y >= 0)
		Player.Physics.Angle = acos(way.X);
	else
		Player.Physics.Angle = -acos(way.X);

	Player.Physics.Update(true); // Изменение позиции игрока
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	// Высчитывание перемещения пули и высчитывание столкновений ///////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < bullet_count; i++)
	{
		if (bullet[i].isExist)
		{
			bullet[i].Physics.Update(false);
			if (bullet[i].Physics.Position.X >= 1.5 || bullet[i].Physics.Position.X <= -1.5 || bullet[i].Physics.Position.Y >= 1.5 || bullet[i].Physics.Position.Y <= -1.5)
				bullet[i].isExist = false;

			// Если произошла колизия, изменим активность пули в нерабочее
			for (int j = 0; j < wall_count; j++)
			{
				if (Collision(bullet[i].Physics.Position, bullet[i].Body.Size, Wall[j].Position, Wall[j].Body.Size))
					bullet[i].isExist = false;
			}
		}
	}


	// Настройки режима стрельбы (ТЕСТ) //////////////////////////////////////////////////////////////////////////////////////////////
	if (isMousePressed)
	{
		//printf("Pressed");
		if (BurstMode == 0)
		{
			CreateBullet();
			BurstMode = 1;
		}
		else if (BurstMode == 2)
			CreateBullet();
	}
	else
	{
		if (BurstMode == 1)
			BurstMode = 0;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glutPostRedisplay(); // Обновляем экран
	glutTimerFunc(timer_update, Update, Value); // Задержка 15 мс перед новым вызовом функции
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
}

// Поворот в соответствии физическому компоненту
void Matrix_Rotate(Vector position, double angle)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(position.X, position.Y, 0);
	glRotated(angle * 180 / M_PI, 0, 0, 1);
	glTranslated(-position.X, -position.Y, 0);
}

// Инициализация главного окна
void initGL(int argc, char **argv)
{
	glutInit(&argc, argv);	// Инициализация glut
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); 	// Установка парометров отрисовки где
																// GLUT_DEPTH - разрешение глубины
																// GLUT_DOUBLE - режим двойной буферизации
																// GLUT_RGBA - цветовой канал(RGB) + альфа канал(А)
	glutInitWindowSize(960, 540);	 // Размер экрана в пикселях
	glutInitWindowPosition(100, 100); // Позиция окна относительно левого верхнего угла(0,0) в пикселях
	glutCreateWindow("Outlaw");	 // Имя окна

	Main_Window_Handle = GetActiveWindow(); // Запоминаем главное окно, что бы в последстыии отключать обработчик клавиш если оно свернуто.


	// Инициализация текстур
	Player.Legs.Load("Legs.png");
	Player.Legs.Size = Vector(0.2, 0.2);
	Player.Body.Load("Body.png");
	Player.Body.Size = Vector(0.4, 0.4);

	Cross.Body.Load("Cross.png");
	for (int i = 0; i < wall_count; i++)
	{
		Wall[i].Body.Load("cobblestone.png");
		Wall[i].Body.Size = Vector(1.0 / 9, 1.0 / 9);
	}
	for (int i = 0; i < bullet_count; i++)
	{
		bullet[i].Body.Load("Bullet.png");
		bullet[i].Body.Size = Vector(0.06, 0.06);
	}

	int k = 0;
	for (int i = 0; i < 18; i++)
	{
		Wall[k].Position.Y = 0.5;
		Wall[k].Position.X = (double)i / 9 - 1.0;
		Wall[k + 1].Position.Y = -0.5;
		Wall[k + 1].Position.X = (double)i / 9 - 1.0;
		k += 2;
	}
	for (int i = 0; i < 10; i++)
	{
		Wall[k].Position.Y = (double)i / 9 - 0.5;
		Wall[k].Position.X = -1.0;
		Wall[k + 1].Position.Y = (double)i / 9 - 0.5;
		Wall[k + 1].Position.X = 1.0;
		k += 2;
	}

	//Биндим клавиши
	key[LEFT].Nominal = KEY_A;
	key[RIGHT].Nominal = KEY_D;
	key[UP].Nominal = KEY_W;
	key[DOWN].Nominal = KEY_S;

	waveOutGetVolume(0, (LPDWORD)&volume);
	glutSetCursor(GLUT_CURSOR_NONE);
}

// Отрисовка
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка буферов глубины и цвета
	glClearColor(0, 0, 0, 1); // Устанавливаем цвет фона

	glEnable(GL_ALPHA_TEST);	// Рразрешаем использовать прозрвачные текстуры
	glAlphaFunc(GL_GREATER, 0.5f); // Порог прорисовки прозрачности
	glEnable(GL_TEXTURE_2D); // Включает двухмерное текстурирование

	Player.Draw(); // Рисуем игрока

	for (int i = 0; i < wall_count; i++)
		Draw_Quad(Wall[i].Position, Wall[i].Body);
	
	//Отрисовка пуль
	for (int i = 0; i < bullet_count; i++)
	{
		if (bullet[i].isExist)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			Matrix_Rotate(bullet[i].Physics.Position, bullet[i].Physics.Angle); // Поворачиваем пулю
			Draw_Quad(bullet[i].Physics.Position, bullet[i].Body); // Рисуем пулю
			glPopMatrix();

		}
	}
	//Отрисовка прицела
	Cross.Body.Size = Vector(0.2, 0.2);
	Draw_Quad(Cross.Physics.Position, Cross.Body);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glutSwapBuffers(); // Замена буфера на вновь отрисованный 
}

// Рисует квадрат в позиции Position и размера Size, где рисование объекта начинается с центра
void Draw_Quad(Vector Position, Sprite Sprite)
{
	glBindTexture(GL_TEXTURE_2D, Sprite.Texture); // Укажем текстуру, далее будет использоваться она
	glBegin(GL_QUADS); // Выбираем метод отрисовки
	glTexCoord2f(0.0, 1.0); glVertex2f(-Sprite.Size.X / 2 + Position.X, -Sprite.Size.Y / 2 + Position.Y); // Задаем координаты декстур и позиции объекта
	glTexCoord2f(1.0, 1.0); glVertex2f(Sprite.Size.X / 2 + Position.X, -Sprite.Size.Y / 2 + Position.Y);
	glTexCoord2f(1.0, 0.0); glVertex2f(Sprite.Size.X / 2 + Position.X, Sprite.Size.Y / 2 + Position.Y);
	glTexCoord2f(0.0, 0.0); glVertex2f(-Sprite.Size.X / 2 + Position.X, Sprite.Size.Y / 2 + Position.Y);
	glEnd();
}

// Регистрация изменения размеров окна
void reshape_win_size(int w, int h)
{
	// Определяем окно просмотра
	if (w * win_height > h * win_width)
	{
		Window.Render_Position.X = (w - h * win_width / win_height) / 2; // Задаем отступ отрисовываемой поверхности по X
		Window.Render_Position.Y = 0;	// Задаем отступ отрисовываемой поверхности по Y
		Window.Render_Size.X = h * win_width / win_height; // Задаем размер отрисовываемой области по X (Ширина)
		Window.Render_Size.Y = h; // Задаем размер отрисовываемой области по Y (Высота)
	}
	else
	{
		Window.Render_Position.Y = (h - w * win_height / win_width) / 2;	// Задаем отступ отрисовываемой поверхности по X
		Window.Render_Position.X = 0;	// Задаем отступ отрисовываемой поверхности по Y
		Window.Render_Size.X = w;	// Задаем размер отрисовываемой области по X (Ширина)
		Window.Render_Size.Y = w * win_height / win_width;	// Задаем размер отрисовываемой области по Y (Высота)
	}
	glViewport(Window.Render_Position.X, Window.Render_Position.Y, Window.Render_Size.X, Window.Render_Size.Y); // Устанавливает область отрисовки внутри окна

	// Сместим соотношение сторон для рендера
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScaled((double)win_height/10, (double)win_width/10, 0);
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
		glutReshapeWindow(Window.Size.X, Window.Size.Y);	 // Установка первоначальных размеров окна
		glutPositionWindow(Window.Position.X, Window.Position.Y);	// Перемещение окна в первоначальное положение
		IsFullScreen = !IsFullScreen;
	}
}

void CreateBullet()
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
		return;
	bullet[k].Physics.Position.X = Player.Physics.Position.X;
	bullet[k].Physics.Position.Y = Player.Physics.Position.Y;
	bullet[k].Physics.Angle = Player.Physics.Angle;
	bullet[k].Physics.Velocity.X = cos(bullet[k].Physics.Angle) * bullet[k].Physics.Speed;
	bullet[k].Physics.Velocity.Y = sin(bullet[k].Physics.Angle) * bullet[k].Physics.Speed;
}

bool Collision(Vector Position1, Vector Size1, Vector Position2, Vector Size2)
{
	bool Collision_X = Position1.X + Size1.X / 2 > Position2.X - Size2.X / 2 && Position1.X - Size1.X / 2 < Position2.X + Size2.X / 2; // Проверим пересечение по X
	bool Collision_Y = Position1.Y - Size1.Y / 2 < Position2.Y + Size2.Y / 2 && Position1.Y + Size1.Y / 2 > Position2.Y - Size2.Y / 2; // Проверим пересечение по Y
	bool Collision = Collision_X && Collision_Y; // Объекты пересекаются

	return Collision;
}

//Функция, которая вызывается при изменении состоянии клавиатуры
//Если code < 0, то нужно пробросить хук дальше, wParam хранит данные о том, нажата клавиша или отпущена, lParam хранит указатель на структуру KBDLLHOOKSTRUCT
LRESULT __stdcall KeybdHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0 && GetActiveWindow() == Main_Window_Handle) //Если нет ошибок и событие вызвано клавиатурой и если хэндл активного окна совпадает с хэндлом нашего окна, то активно наше оконо и можно обрабатывать нажатия
	{
		//Данная структура хранит информацию о клавише
		KBDLLHOOKSTRUCT *KEY = (KBDLLHOOKSTRUCT*)lParam; //Получаем указатель на структуру данных о нажатой клавише

		for (int i = 0; i < gamekey_size; i++) // Проверяем игровые клавиши на нажитие, где sizeof(gamekey) - доступное кол-во клавиш
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
			Cross.Physics.Position.X = ((MOUSE->pt.x - Window.Position.X - Window.Render_Position.X) / Window.Render_Size.X * 2 - 1.0) * 10 / win_height;
			Cross.Physics.Position.Y = (-(MOUSE->pt.y - Window.Position.Y - 20 - Window.Render_Position.Y) / Window.Render_Size.Y * 2 + 1.0) * 10 / win_width;
		}
		if (wParam == WM_LBUTTONDOWN)
		{
			isMousePressed = true;
		}
		if (wParam == WM_LBUTTONUP)
		{
			isMousePressed = false;
		}
		if (wParam == WM_RBUTTONDOWN)
		{
			if (BurstMode == 2)
				BurstMode = 0;
			else
				BurstMode = 2;
		}
	}
	return CallNextHookEx(Keyboard_Hook, code, wParam, lParam); //Пробрасываем хук дальше
}
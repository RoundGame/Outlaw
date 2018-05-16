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

const int bullet_count = 10;
const int wall_count = 67;
const int hp_count = 5;
Static_Object pick;
Static_Object pick2;
Object bullet[bullet_count];
Static_Object Cross;
Static_Object HP[hp_count];
Character Player; // Создаем игрока
Character Enemy; // Создаем врага
Static_Object Floor;
Static_Object Wall[wall_count];
int volume; // Тестовая переменная громкости звука

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

	if (Player.HP <= 0)
	{
		glutPostRedisplay(); // Обновляем экран
		glutTimerFunc(timer_update, Update, Value); // Задержка 15 мс перед новым вызовом функции
		return;
	}

	// Высчитывание перемещения игрока //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Player.Physics.Acceleration.X = -1 * key[LEFT].isPressed + key[RIGHT].isPressed; // Получаем направление движения по X
	Player.Physics.Acceleration.Y = -1 * key[DOWN].isPressed + key[UP].isPressed;	// Получаем направление движения по Y
	Player.Physics.Acceleration = Player.Physics.Acceleration.GetNormalize();
	Player.Set_Legs_Direction();


	Player.Use_Collisions(Wall, wall_count);
	Player.Target_To(Cross.Position, Window.Render_Size);
	Player.Physics.Update(true); // Изменение позиции игрока
	
	if (Collision(Player.Physics.Position, Player.Legs.Size, pick.Position, pick.Body.Size) && pick.isExist)
	{
		pick.isExist = false;
		Player.Physics.Speed *= 1.5f;
	}

	if (Collision(Player.Physics.Position, Player.Legs.Size, pick2.Position, pick2.Body.Size) && pick2.isExist)
	{
		pick2.isExist = false;
		Player.Body.Size = Vector(0.3, 0.3);
		Player.Legs.Size = Vector(0.15, 0.15);
		Player.Attack.Size = Vector(0.14, 0.14);
	}
	
	double max = wall_count * 100, X = 0, Y = 0;
	for (double x = Enemy.Physics.Position.X - 0.1; x <= Enemy.Physics.Position.X + 0.1; x += 0.1)
	{
		for (double y = Enemy.Physics.Position.Y - 0.1; y <= Enemy.Physics.Position.Y + 0.1; y += 0.1)
		{
			if (y != Enemy.Physics.Position.Y || x != Enemy.Physics.Position.X)
			{
				Vector temp = Vector(Player.Physics.Position.X - x, Player.Physics.Position.Y - y);
				double len = 0.15 * temp.GetLength() * wall_count;
				for (int i = 0; i < wall_count; i++)
				{
					temp = Vector(Wall[i].Position.X - x, Wall[i].Position.Y - y);
					len += 0.2 / temp.GetLength();
				}
				if (len < max)
				{
					max = len;
					X = x;
					Y = y;
				}
			}
		}
	}
	Vector EnemyWay = Vector(X - Enemy.Physics.Position.X, Y - Enemy.Physics.Position.Y);
	Vector FromEnemyToPlayer = Vector(Player.Physics.Position.X - Enemy.Physics.Position.X, Player.Physics.Position.Y - Enemy.Physics.Position.Y);
	EnemyWay = EnemyWay.GetNormalize();
	if (Enemy.HP > 0)
	{
		if (FromEnemyToPlayer.GetLength() > 0.1)
		{
			Enemy.Physics.Acceleration.X = EnemyWay.X;
			Enemy.Physics.Acceleration.Y = EnemyWay.Y;
		}
		else
		{
			Enemy.Physics.Acceleration = Vector(0, 0);
			Enemy.isAttack = true;
			Enemy.isKick = true;
		}
		Enemy.Set_Legs_Direction();

		Enemy.Use_Collisions(Wall, wall_count);
		Enemy.Target_To(Player.Physics.Position, Window.Render_Size);
		Enemy.Physics.Update(true);

		if (Collision(Player.Physics.Position, Player.Legs.Size, Enemy.Physics.Position, Enemy.Legs.Size))
		{
			if (Player.isKick)
			{
				Enemy.Physics.Velocity.X -= FromEnemyToPlayer.GetNormalize().X * Player.Knock_Back;
				Enemy.Physics.Velocity.Y -= FromEnemyToPlayer.GetNormalize().Y * Player.Knock_Back;
				Enemy.HP -= rand() % 7 + 7;
				Player.isKick = false;
			}
			if (Enemy.isKick && !Player.isInvulnerability)
			{
				Player.Physics.Velocity.X += FromEnemyToPlayer.GetNormalize().X * Enemy.Knock_Back;
				Player.Physics.Velocity.Y += FromEnemyToPlayer.GetNormalize().Y * Enemy.Knock_Back;
				Player.HP -= 20;
				Player.isInvulnerability = true;
				Enemy.isKick = false;
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Высчитывание перемещения пули и высчитывание столкновений ///////////////////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < bullet_count; i++)
	{
		if (bullet[i].isExist)
		{
			bullet[i].Physics.Update(false);
			if (bullet[i].Physics.Position.X >= 1.5 || bullet[i].Physics.Position.X <= -1.5 || bullet[i].Physics.Position.Y >= 1.5 || bullet[i].Physics.Position.Y <= -1.5)
				bullet[i].isExist = false;

			if (Enemy.HP > 0 && Collision(bullet[i].Physics.Position, bullet[i].Body.Size, Enemy.Physics.Position, Enemy.Legs.Size))
			{
				bullet[i].isExist = false;
				Enemy.HP -= rand() % 7 + 15;
			}
			// Если произошла колизия, изменим активность пули в нерабочее
			for (int j = 0; j < wall_count; j++)
			{
				if (Collision(bullet[i].Physics.Position, bullet[i].Body.Size, Wall[j].Position, Wall[j].Body.Size))
					bullet[i].isExist = false;
			}
		}
	}

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
	if (Player.HP <= 0)
	{
		glutTimerFunc(timer_update, Animation, Value); // Задержка 15 мс перед новым вызовом функции
		return;
	}
	Player.Animation(); // Анимация игрока
	Enemy.Animation(); //Анимация врага
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


	// Инициализация объектов (+загрузка текстур)
	Player.Legs.Load("textures/Legs.png"); // Игрок
	Player.Legs.Size = Vector(0.2, 0.2);
	Player.Body.Load("textures/Body_Gun.png");
	Player.Body.Size = Vector(0.4, 0.4);
	Player.Death.Load("textures/Death.png");
	Player.Death.Size = Vector(0.35, 0.35);
	Player.Attack.Load("textures/Attack.png");
	Player.Attack.Size = Vector(0.18, 0.18);
	Player.Physics.Position = Vector(-0.5, 0.02);
	Player.Physics.Speed = 0.2;

	pick.Body.Load("textures/boots.png"); // Поднимаемые предметы
	pick.Body.Size = Vector(0.1, 0.1);
	pick.Position = Vector(0.5, 0.01);
	pick.isExist = true;

	pick2.Body.Load("textures/dye_powder_cyan.png");
	pick2.Body.Size = Vector(0.1, 0.1);
	pick2.Position = Vector(0.2, 0.41);
	pick2.isExist = true;

	Floor.Body.Load("textures/planks.png");
	Floor.Body.Size = Vector(0.1, 0.1);

	Enemy.Legs.Load("textures/Legs.png"); // Враг
	Enemy.Legs.Size = Vector(0.2, 0.2);
	Enemy.Body.Load("textures/Body.png");
	Enemy.Body.Size = Vector(0.4, 0.4);
	Enemy.Death.Load("textures/Death.png");
	Enemy.Death.Size = Vector(0.35, 0.35);
	Enemy.Attack.Load("textures/Attack.png");
	Enemy.Attack.Size = Vector(0.18, 0.18);
	Enemy.Physics.Position = Vector(0.5, 0.0);
	Enemy.Physics.Speed = 0.1;
	Enemy.Knock_Back = 0.3;

	Cross.Body.Load("textures/Cross.png"); // Прицел
	Cross.Body.Size = Vector(0.1, 0.1);

	for (int i = 0; i < hp_count; i++)
	{
		HP[i].Body.Load("textures/hp.png");
		HP[i].Body.Size = Vector(0.07, 0.07);
		HP[i].Position = Vector(-10 / (double)win_height + 0.06 + i * 0.08, 10 / (double)win_width - 0.06);
	}

	for (int i = 0; i < wall_count; i++)
	{
		Wall[i].Body.Load("textures/cobblestone.png");
		Wall[i].Body.Size = Vector(1.0 / 9, 1.0 / 9);
	}
	for (int i = 0; i < bullet_count; i++)
	{
		bullet[i].Body.Load("textures/Bullet.png");
		bullet[i].Body.Size = Vector(0.1, 0.05);
		bullet[i].Physics.Speed = 0.4;
	}

	int k = 0;
	for (int i = 0; i < 20; i++)
	{
		Wall[k].Position.Y = 10 / (double)win_width;
		Wall[k].Position.X = (double)i / 9 - 10 / (double)win_height;
		Wall[k + 1].Position.Y = -10 / (double)win_width;
		Wall[k + 1].Position.X = (double)i / 9 - 10 / (double)win_height;
		k += 2;
	}
	for (int i = 0; i < 12; i++)
	{
		Wall[k].Position.Y = (double)i / 9 - 10 / (double)win_width;
		Wall[k].Position.X = -10 / (double)win_height;
		Wall[k + 1].Position.Y = (double)i / 9 - 10 / (double)win_width;
		Wall[k + 1].Position.X = 10 / (double)win_height;
		k += 2;
	}
	Wall[k].Position.Y = 1.0 / 9;
	Wall[k + 1].Position.Y = -1.0 / 9;

	//Биндим клавиши
	key[LEFT].Nominal1 = KEY_A;
	key[RIGHT].Nominal1 = KEY_D;
	key[UP].Nominal1 = KEY_W;
	key[DOWN].Nominal1 = KEY_S;

	key[LEFT].Nominal2 = VK_LEFT;
	key[RIGHT].Nominal2 = VK_RIGHT;
	key[UP].Nominal2 = VK_UP;
	key[DOWN].Nominal2 = VK_DOWN;

	waveOutGetVolume(0, (LPDWORD)&volume);
	glutSetCursor(GLUT_CURSOR_NONE);
	srand(time(0));
}

// Отрисовка
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка буферов глубины и цвета
	glClearColor(0, 0, 0, 1); // Устанавливаем цвет фона



	glEnable(GL_ALPHA_TEST);	// Рразрешаем использовать прозрвачные текстуры
	glAlphaFunc(GL_GREATER, 0.5f); // Порог прорисовки прозрачности
	glEnable(GL_TEXTURE_2D); // Включает двухмерное текстурирование

	// Рисуем пол
	for (int i = -11; i <= 11; i++)
	{
		for (int j = -6; j <= 6; j++)
			Draw_Quad(Vector((float)i / 10.0f, (float)j / 10.0f), Floor.Body);
	}


	if (pick.isExist)
		Draw_Quad(pick.Position, pick.Body);

	if (pick2.isExist)
		Draw_Quad(pick2.Position, pick2.Body);

	Enemy.Draw();
	Player.Draw();// Рисуем игрока

	//Отрисовка стен
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
	Draw_Quad(Cross.Position, Cross.Body);

	//Отрисовка HP
	for (int i = 0; i < hp_count; i++)
	{
		if (Player.HP / 20 > i)
			Draw_Quad(HP[i].Position, HP[i].Body);
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);

	if (Player.HP > 0)
	{
		if (Enemy.HP > 0)
		{
			char text[4];
			_itoa_s((int)Enemy.HP, text, 10);
			for (int i = 0; i < 3; i++)
			{
				glRasterPos2d(Enemy.Physics.Position.X - 0.08 + i * 0.04, Enemy.Physics.Position.Y + 0.06);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
			}
			glRasterPos2d(Enemy.Physics.Position.X + 0.04, Enemy.Physics.Position.Y + 0.06);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'H');
			glRasterPos2d(Enemy.Physics.Position.X + 0.08, Enemy.Physics.Position.Y + 0.06);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'P');
		}
	}
	else
	{
		char text[10] = "Game Over";
		for (int i = 0; i < 10; i++)
		{
			glRasterPos2d(-0.16 + i * 0.04, 0.0);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
		}
	}

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
			if (key[i].Nominal1 == KEY->vkCode || key[i].Nominal2 == KEY->vkCode)
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
			Cross.Position.X = ((MOUSE->pt.x - Window.Position.X - Window.Render_Position.X) / Window.Render_Size.X * 2 - 1.0) * 10 / win_height;
			Cross.Position.Y = (-(MOUSE->pt.y - Window.Position.Y - 20 - Window.Render_Position.Y) / Window.Render_Size.Y * 2 + 1.0) * 10 / win_width;
		}
		if (wParam == WM_LBUTTONDOWN)
		{
			if (!Player.isAttack && Player.HP > 0)
				CreateBullet();
			//PlaySoundA("pistol.wav", NULL, SND_ASYNC | SND_FILENAME);
		}
		if (wParam == WM_RBUTTONDOWN)
		{
			Player.isAttack = true;
			Player.isKick = true;
		}
	}
	return CallNextHookEx(Keyboard_Hook, code, wParam, lParam); //Пробрасываем хук дальше
}
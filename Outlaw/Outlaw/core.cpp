#include "core.h"
#include <soil.h>
#include <cstdio>

level Map;
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
const int hp_count = 5;
const int wall_count = 67;
const int ice_count = 8;
Object bullet[bullet_count];
Static_Object pick;
Static_Object pick2;
Static_Object Cross;
Static_Object Floor;
Static_Object HP[hp_count];
Static_Object Wall[wall_count];
Static_Object Ice[ice_count];


Static_Object Map_Back;
Tile Level_Tile[level_size * level_size];
int tile_count = level_size * 4;
double Map_Size = 0.04;

Character Player; // Создаем игрока
Character Enemy; // Создаем врага

enum ButtonType { BUTTON_NEW_GAME = 0, BUTTON_SETTINGS, BUTTON_EXIT, BUTTON_BACK, BUTTON_CONTINUE, button_count };
int currentMenu = -1, prevMenu = -1;
Static_Object Menu_BackGround;
Static_Object Menu_Text;
Static_Object Settings_Text;
Static_Object Button[button_count];
Static_Object Cursor;
int currentButton = -1;

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

	//Если открыто меню
	if (currentMenu != 0)
	{
		if (currentMenu == 1 || currentMenu == -1)
		{
			Menu_Text.isExist = true;
			Settings_Text.isExist = false;
			Button[BUTTON_CONTINUE].isExist = true;
			Button[BUTTON_NEW_GAME].isExist = true;
			Button[BUTTON_SETTINGS].isExist = true;
			Button[BUTTON_EXIT].isExist = true;
			Button[BUTTON_BACK].isExist = false;
		}
		if (currentMenu == 2)
		{
			Menu_Text.isExist = false;
			Settings_Text.isExist = true;
			Button[BUTTON_CONTINUE].isExist = false;
			Button[BUTTON_NEW_GAME].isExist = false;
			Button[BUTTON_SETTINGS].isExist = false;
			Button[BUTTON_EXIT].isExist = false;
			Button[BUTTON_BACK].isExist = true;
		}
		currentButton = -1;
		Cursor.isExist = false;
		for (int i = 0; i < button_count && !Cursor.isExist; i++)
		{
			if (Button[i].isExist && Collision(Button[i].Position, Button[i].Body.Size, Cross.Position, Vector(0.0, 0.0)))
			{
				if (i != BUTTON_CONTINUE || currentMenu != -1)
				{
					Cursor.isExist = true;
					Cursor.Position = Button[i].Position;
					currentButton = i;
				}
			}
		}
		glutPostRedisplay(); // Обновляем экран
		glutTimerFunc(timer_update, Update, Value); // Задержка 15 мс перед новым вызовом функции
		return;
	}

	//Изменение размера карты
	if (key[MINIMAP].isPressed)
	{
		if (Map_Size == 0.04)
		{
			Map_Size = 0.08;
			BuildMap(Map_Size, false);
		}
	}
	else
	{
		if (Map_Size == 0.08)
		{
			Map_Size = 0.04;
			BuildMap(Map_Size, false);
		}
	}

	//Если мертвы, то больше не обновляем остальную информацию
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

	//Проверка, что мы на льду
	Player.Physics.Boost = 5;
	Enemy.Physics.Boost = 5;
	for (int i = 0; i < ice_count; i++)
	{
		if (Collision(Player.Physics.Position, Player.Legs.Size, Ice[i].Position, Ice[i].Body.Size))
			Player.Physics.Boost = 0.5;
		if (Collision(Enemy.Physics.Position, Enemy.Legs.Size, Ice[i].Position, Ice[i].Body.Size))
			Enemy.Physics.Boost = 0.5;
	}

	//Проверка, что мы взяли пикапы
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

	//Обход стен врагом, реализованный с помощью метода потенциальных полей
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
void initGL(int argc, char **argv, bool isNewWindow)
{
	if (isNewWindow)
	{
		glutInit(&argc, argv);	// Инициализация glut
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); 	// Установка парометров отрисовки где
																	// GLUT_DEPTH - разрешение глубины
																	// GLUT_DOUBLE - режим двойной буферизации
																	// GLUT_RGBA - цветовой канал(RGB) + альфа канал(А)
		glutInitWindowSize(960, 540);	 // Размер экрана в пикселях
		glutInitWindowPosition(100, 100); // Позиция окна относительно левого верхнего угла(0,0) в пикселях
		glutCreateWindow("Outlaw");	 // Имя окна
	}

	Main_Window_Handle = GetActiveWindow(); // Запоминаем главное окно, что бы в последствии отключать обработчик клавиш если оно свернуто.

	//Инициализация меню
	Menu_BackGround.Body.Load("textures/Menu/BackGround.png");
	Menu_BackGround.Body.Size = Vector(20 / (double)win_height, 20 / (double)win_width);
	Menu_BackGround.Position = Vector(0.0, 0.0);
	Menu_Text.Body.Load("textures/Menu/Menu_Text.png");
	Menu_Text.Body.Size = menu_text_size;
	Menu_Text.Position = menu_text_position;
	Settings_Text.Body.Load("textures/Menu/Settings_Text.png");
	Settings_Text.Body.Size = menu_text_size;
	Settings_Text.Position = menu_text_position;
	Button[BUTTON_NEW_GAME].Body.Load("textures/Menu/New_Game.png");
	Button[BUTTON_NEW_GAME].Body.Size = menu_button_size;
	Button[BUTTON_NEW_GAME].Position = Vector(-1 + menu_button_size.X/3 + menu_button_ident, -0.02);
	Button[BUTTON_SETTINGS].Body.Load("textures/Menu/Settings.png");
	Button[BUTTON_SETTINGS].Body.Size = menu_button_size;
	Button[BUTTON_SETTINGS].Position = Vector(-1 + menu_button_size.X / 3 + menu_button_ident, -0.14);
	Button[BUTTON_EXIT].Body.Load("textures/Menu/Exit.png");
	Button[BUTTON_EXIT].Body.Size = menu_button_size;
	Button[BUTTON_EXIT].Position = Vector(-1 + menu_button_size.X / 3 + menu_button_ident, -0.26);
	Button[BUTTON_BACK].Body.Load("textures/Menu/Back.png");
	Button[BUTTON_BACK].Body.Size = menu_button_size;
	Button[BUTTON_BACK].Position = Vector(-1 + menu_button_size.X / 3 + menu_button_ident, -0.26);
	if (currentMenu == -1)
		Button[BUTTON_CONTINUE].Body.Load("textures/Menu/Continue_R.png");
	else
		Button[BUTTON_CONTINUE].Body.Load("textures/Menu/Continue.png");
	Button[BUTTON_CONTINUE].Body.Size = menu_button_size;
	Button[BUTTON_CONTINUE].Position = Vector(-1 + menu_button_size.X / 3 + menu_button_ident, 0.1);
	Cursor.Body.Load("textures/Menu/Cursor.png");
	Cursor.Body.Size = menu_button_size;
	Cursor.Position = Vector(-1 + menu_button_size.X / 3 + menu_button_ident, 0.0);
	if (currentMenu == -1)
		return;

	//Генерация карты и уровня
	Map = level();
	for (int i = 0; i < level_size - 3; i++)
		Map.generation();
	Map.draw();

	Map.draw(Map.current);
	BuildMap(Map_Size, true);

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
	Player.HP = 100;

	pick.Body.Load("textures/boots.png"); // Поднимаемые предметы
	pick.Body.Size = Vector(0.1, 0.1);
	pick.Position = Vector(0.6, 0.01);
	pick.isExist = true;

	pick2.Body.Load("textures/dye_powder_cyan.png");
	pick2.Body.Size = Vector(0.1, 0.1);
	pick2.Position = Vector(0.3, 0.41);
	pick2.isExist = true;

	Floor.Body.Load("textures/planks.png");
	Floor.Body.Size = Vector(1.0 / 9, 1.0 / 9);

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
	Enemy.HP = 100;

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
	for (int i = 0; i < ice_count; i++)
	{
		Ice[i].Body.Load("textures/ice.png");
		Ice[i].Body.Size = Vector(1.0 / 9, 1.0 / 9);
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
	for (int i = 0; i < ice_count; i += 2)
	{
		Ice[i].Position.X = -(double)i / 18 - 3.5 / 9;
		Ice[i].Position.Y = -2.0 / 9;
		Ice[i + 1].Position.X = -(double)i / 18 - 3.5 / 9;
		Ice[i + 1].Position.Y = -3.0 / 9;
	}

	//Биндим клавиши
	key[LEFT].Nominal1 = KEY_A;
	key[RIGHT].Nominal1 = KEY_D;
	key[UP].Nominal1 = KEY_W;
	key[DOWN].Nominal1 = KEY_S;
	key[MINIMAP].Nominal1 = VK_TAB;
	key[GAMEMENU].Nominal1 = VK_ESCAPE;
	key[FULLSCREEN].Nominal1 = VK_F11;

	key[LEFT].Nominal2 = VK_LEFT;
	key[RIGHT].Nominal2 = VK_RIGHT;
	key[UP].Nominal2 = VK_UP;
	key[DOWN].Nominal2 = VK_DOWN;
	key[MINIMAP].Nominal2 = KEY_UNKNOWN;
	key[GAMEMENU].Nominal2 = KEY_UNKNOWN;
	key[FULLSCREEN].Nominal2 = KEY_UNKNOWN;

	glutSetCursor(GLUT_CURSOR_NONE);
	srand(time(0));
}

void BuildMap(double Map_Size, bool reloadTexture)
{
	int k = 0;
	for (int x = 0; x < level_size; x++)
	{
		for (int y = 0; y < level_size; y++)
		{
			if (Map.box[y][x] == NORMAL)
			{
				if (reloadTexture)
				{
					if (x == level_size / 2 && y == level_size / 2)
						Level_Tile[k].isStudy = true;
					else
						Level_Tile[k].isStudy = false;
				}
				Level_Tile[k].isExist = true;
				Level_Tile[k].Position = Vector(10 / (double)win_height - (level_size - 0.5 - x) * Map_Size, 10 / (double)win_width - (y + 0.5) * Map_Size);
				if (reloadTexture)
				{
					Level_Tile[k].First.Load("textures/Normal_Level1.png");
					Level_Tile[k].Second.Load("textures/Normal_Level2.png");
				}
				Level_Tile[k].First.Size = Vector(Map_Size, Map_Size);
				Level_Tile[k].Second.Size = Vector(Map_Size, Map_Size);
				k++;
			}
		}
	}
	tile_count = k;
	Map_Back.Body.Load("textures/Map_Back.png");
	Map_Back.Body.Size = Vector(level_size * Map_Size, level_size * Map_Size);
	Map_Back.Position = Vector(10 / (double)win_height - level_size * Map_Size / 2, 10 / (double)win_width - level_size * Map_Size / 2);
}

// Отрисовка
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка буферов глубины и цвета
	glClearColor(0, 0, 0, 1); // Устанавливаем цвет фона


	glEnable(GL_ALPHA_TEST);	// Рразрешаем использовать прозрвачные текстуры
	glAlphaFunc(GL_GREATER, 0.5f); // Порог прорисовки прозрачности
	glEnable(GL_TEXTURE_2D); // Включает двухмерное текстурирование

	if (currentMenu != 0)
	{
		//Отрисовка меню
		Draw_Quad(Menu_BackGround.Position, Menu_BackGround.Body);
		if (Menu_Text.isExist)
			Draw_Quad(Menu_Text.Position, Menu_Text.Body);
		if (Settings_Text.isExist)
			Draw_Quad(Settings_Text.Position, Settings_Text.Body);
		for (int i = 0; i < button_count; i++)
		{
			if (Button[i].isExist)
				Draw_Quad(Button[i].Position, Button[i].Body);
		}
		if (Cursor.isExist)
			Draw_Quad(Cursor.Position, Cursor.Body);

		//Отрисовка прицела
		Draw_Quad(Cross.Position, Cross.Body);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		glutSwapBuffers(); // Замена буфера на вновь отрисованный 
		return;
	}

	// Рисуем пол
	for (unsigned __int8 i = 0; i < room_h; i++)
	{
		for (unsigned __int8 j = 0; j < room_w; j++)
		{
			if (Map.current->box[i][j] == room_wall)
			{
				Draw_Quad(Vector((float)(Wall[0].Body.Size.X * (j - room_w / 2) + 0.05), (float)(-Wall[0].Body.Size.X) * (i - room_h / 2) - 0.05), Wall[0].Body);
			}
			if (Map.current->box[i][j] != room_wall)
			{
				Draw_Quad(Vector((float)(Wall[0].Body.Size.X * (j - room_w / 2) + 0.05), (float)(-Wall[0].Body.Size.X) * (i - room_h / 2) - 0.05), Floor.Body);
			}
			if (Map.current->box[i][j] == room_gift)
			{
				Draw_Quad(Vector((float)(Wall[0].Body.Size.X * (j - room_w / 2) + 0.05), (float)(-Wall[0].Body.Size.X) * (i - room_h / 2) - 0.05), pick.Body);
			}
		}
	}

	//Отрисовка льда
	//for (int i = 0; i < ice_count; i++)
	//	Draw_Quad(Ice[i].Position, Ice[i].Body);

	//Отрисовка пикапов

	if (pick.isExist)
		Draw_Quad(pick.Position, pick.Body);

	if (pick2.isExist)
		Draw_Quad(pick2.Position, pick2.Body);

	Enemy.Draw(); // Рисуем врага
	Player.Draw();// Рисуем игрока

	//Отрисовка стен
	//for (int i = 0; i < wall_count; i++)
	//	Draw_Quad(Wall[i].Position, Wall[i].Body);

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

	//Отрисовка миникарты
	Draw_Quad(Map_Back.Position, Map_Back.Body);
	for (int i = 0; i < tile_count; i++)
	{
		if (Level_Tile[i].isExist)
		{
			if (Level_Tile[i].isStudy)
				Draw_Quad(Level_Tile[i].Position, Level_Tile[i].Second);
			else
				Draw_Quad(Level_Tile[i].Position, Level_Tile[i].First);
		}
	}

	//Отрисовка HP
	for (int i = 0; i < hp_count; i++)
	{
		if (Player.HP / 20 > i)
			Draw_Quad(HP[i].Position, HP[i].Body);
	}

	//Отрисовка прицела
	Draw_Quad(Cross.Position, Cross.Body);

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
	glScaled((double)win_height / 10, (double)win_width / 10, 0);
}

/* Состояние прилржения
   false - окно
   true - полный экран */
bool IsFullScreen = false;
Vector prevWinSize, prevWinPos;

void SetFullScreen() //Функция установки полного экрана или возвращения в окно
{
	if (!IsFullScreen)
	{
		prevWinSize = Window.Size;
		prevWinPos = Window.Position;
		glutFullScreen();	// Запуск полноэкранного режима
		IsFullScreen = !IsFullScreen;
	}
	else
	{
		glutReshapeWindow(prevWinSize.X - 16, prevWinSize.Y - 39);	 // Установка первоначальных размеров окна
		glutPositionWindow(prevWinPos.X, prevWinPos.Y);	// Перемещение окна в первоначальное положение
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
		if (wParam == WM_KEYUP)
		{
			if ((key[GAMEMENU].Nominal1 == KEY->vkCode || key[GAMEMENU].Nominal2 == KEY->vkCode))
			{
				if (currentMenu == 0)
					currentMenu = 1;
				else if (currentMenu != -1)
					currentMenu = prevMenu;
			}
			if ((key[FULLSCREEN].Nominal1 == KEY->vkCode || key[FULLSCREEN].Nominal2 == KEY->vkCode))
				SetFullScreen();
		}
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
			if (currentMenu == 0)
			{
				if (!Player.isAttack && Player.HP > 0)
					CreateBullet();
			}
			else
			{
				switch (currentButton)
				{
				case BUTTON_CONTINUE:
					currentMenu = 0;
					break;
				case BUTTON_NEW_GAME:
					currentMenu = 0;
					initGL(NULL, NULL, false);
					break;
				case BUTTON_SETTINGS:
					prevMenu = currentMenu;
					currentMenu = 2;
					break;
				case BUTTON_EXIT:
					Save();
					break;
				case BUTTON_BACK:
					currentMenu = prevMenu;
					break;
				default:
					break;
				}
			}
		}
		if (wParam == WM_RBUTTONDOWN)
		{
			if (Player.HP > 0 && currentMenu == 0)
			{
				Player.isAttack = true;
				Player.isKick = true;
			}
		}
	}
	return CallNextHookEx(Keyboard_Hook, code, wParam, lParam); //Пробрасываем хук дальше
}
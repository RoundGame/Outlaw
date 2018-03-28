#include "core.h"
#include "key.h"
#include <soil.h>
#include <cstdio>
#include <cmath>

const double speed = 0.03; //Множитель скорости изменения координат (скорость передвижения)

//текстура
unsigned int Textures[6]; // Максимально доступное кол-во текстур
double CurrentFrame = 0; //Текуший кадр анимации
double CurrentAnimation = 0; //Текущая анимация 0 - стоим, 1 - идем

struct Vector
{
	double X, Y, Len;
	Vector()
	{
		X = 0;
		Y = 0;
		Len = 0;
	}
	Vector(double x, double y) //Инициализировать вектор с данными X и Y
	{
		X = x;
		Y = y;
		Len = GetLength();
	}
	double GetLength() //Получить длину текущего вектора
	{
		double len = sqrt(X * X + Y * Y);
		return len;
	}
	Vector GetNormalize() //Получить нормализированный вектор от текущего вектора
	{
		double len = GetLength();
		if (len == 0)
			return Vector();
		double x = X / len;
		double y = Y / len;
		return Vector(x, y);
	}
};

struct Player 
{
	Vector Position, Velocity;
};
Player MainPlayer = Player();

void Update(int value) //Обробатываем данные позиции игрока и его скорости
{
	glutPostRedisplay(); //Обновляем экран
	Vector velocity = MainPlayer.Velocity.GetNormalize(); //Нормализуем вектор скорости
	if (velocity.Len != 0) //Если есть скорость то
	{
		CurrentAnimation = 1; //Включаем анимацию передвижения
		MainPlayer.Position.X += velocity.X * speed; //Добавляем к вектору игрока вектор его скорости
		MainPlayer.Position.Y += velocity.Y * speed; 
	}
	else //Иначе
	{
		CurrentFrame = 0; //Текущий кадр - нулевой (стоим)
		CurrentAnimation = 0; //Текущая анимация нулевая (стоим)
	}
	glutTimerFunc(20, Update, 0); //Задержка 20 мс перед новым вызовом функции
}

void Animation(int value) //Обновляем Кадры исходя из текущей анимации
{
	if (CurrentAnimation == 1)
	{
		CurrentFrame++;
		if (CurrentFrame > 4) //В анимации пять кадров, поэтому сбрасываем счетчик на 0, как только он перевалил за 4
			CurrentFrame = 0;
	}
	glutTimerFunc(100, Animation, 0); //Задержка 100 мс перед новым вызовом функции
}

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

	glBindTexture(GL_TEXTURE_2D, Textures[0]); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, 0.7);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, 0.7);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, 0.7);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, 0.7);
	glEnd(); // Конец обьекта рисуемого треугольниками

	glBindTexture(GL_TEXTURE_2D, Textures[1]); // Привязываем текстуру, далее будет использоваться она, до новой привязки
	glBegin(GL_QUADS); // Начало обьекта рисуемого треугольниками
	glTexCoord2f(CurrentFrame / 5, 0.5); glVertex2f(-0.25 + MainPlayer.Position.X, -0.25 + MainPlayer.Position.Y);
	glTexCoord2f(CurrentFrame / 5, 0.0); glVertex2f(-0.25 + MainPlayer.Position.X, 0.25 + MainPlayer.Position.Y);
	glTexCoord2f(CurrentFrame / 5 + 0.2, 0.0); glVertex2f(0.25 + MainPlayer.Position.X, 0.25 + MainPlayer.Position.Y);
	glTexCoord2f(CurrentFrame / 5 + 0.2, 0.5); glVertex2f(0.25 + MainPlayer.Position.X, -0.25 + MainPlayer.Position.Y);
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

void NormalKeysUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case KEY_ESC: exit(0);
		break;
	case KEY_A: //Если отпущена клавиша A, то останавливаемся на оси X, так как двигались налево
		MainPlayer.Velocity.X = 0;
		break;
	case KEY_D: //Если отпущена клавиша D, то останавливаемся на оси X, так как двигались направо
		MainPlayer.Velocity.X = 0;
		break;
	case KEY_W: //Если отпущена клавиша W, то останавливаемся на оси Y, так как двигались вверх
		MainPlayer.Velocity.Y = 0;
		break;
	case KEY_S: //Если отпущена клавиша S, то останавливаемся на оси Y, так как двигались вниз
		MainPlayer.Velocity.Y = 0;
		break;
	default:
		break;
	}
}

void NormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case KEY_ESC: exit(0);
		break;
	case KEY_A: //Если нажата клавиша A, то вектор скорости по X ставим равным -1, так как двигаемся налево
		MainPlayer.Velocity.X = -1; 
		break;
	case KEY_D: //Если нажата клавиша D, то вектор скорости по X ставим равным 1, так как двигаемся направо
		MainPlayer.Velocity.X = 1;
		break;
	case KEY_W: //Если нажата клавиша W, то вектор скорости по Y ставим равным 1, так как двигаемся вверх
		MainPlayer.Velocity.Y = 1;
		break;
	case KEY_S: //Если нажата клавиша S, то вектор скорости по X ставим равным -1, так как двигаемся вниз
		MainPlayer.Velocity.Y = -1;
		break;
	default:
		break;
	}
}

void SpecialKeys(int key, int x, int y)
{

}
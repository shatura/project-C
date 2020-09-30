#include <cstdlib>
#include <ctime>
#include <cmath>
#include <complex>
#include <GL/glut.h>
#define WIDTH 800
#define HEIGHT 640
#define RANGE 80
#define MAX 50
#define INDEX 0.01
#define RE_C 1.07
#define IM_C 0.0001
 
using namespace std;
 
//рисование точки с координатами (x, y) передаваемым в функцию цветом(RGB)
void draw_dot ( int x, int y, GLubyte *clr ) {
	glColor3ub (clr[0], clr[1], clr[2]); //цвет рисования
	//на месте каждой вершины, указанной glVertex рисуется точка
	glBegin ( GL_POINTS );
	glVertex2i ( x, y );
	glEnd ();
}
 
void draw_biomorph () {
	complex <double> z; //z будет возводиться в куб
	//константа, на каждой итерации будет добавляться к z, возведенному в куб
	const complex <double> constant( RE_C, IM_C );
	double re_z, im_z; //модули действительной и мнимой частей z на текущей итерации
	//clr_black-цвет фона, new_clr-цвет биоморфа(RGB)
	GLubyte clr_black[] = { 0, 0, 0 }, new_clr[3];
//инициализация начала последовательности, генерируемой rand() текущим временем  в секундах
	srand ( time (0) );
	//определяем z для каждой точки окна
	for ( int x = -WIDTH / 2; x < WIDTH / 2; x++ ) {
		for ( int y = -HEIGHT / 2; y < HEIGHT / 2; y++ ) {
//преобразуем координаты текущей точки, re_z <= 4, im_z <= 3.2
			z.real ( x * INDEX );
			z.imag ( y * INDEX );
			//модули действительной и мнимой частей z
			re_z = fabs ( z.real () );
			im_z = fabs ( z.imag () );
//возводим z в куб, пока модуль действительной, мнимой части или модуль самого z
//не превзойдут максимального числа из указанного диапазона(RANGE)
//MAX ограничивает число итераций для случая,
//если z убывает при каждом возведении в куб(т.е. никогда не достигнет MAX)
			for ( int i = 0; ( re_z < RANGE || im_z < RANGE || abs(z) < RANGE ) && i < MAX; i++ ) {
				//возводим предыдущее значение z в куб добавляя константу
				z = pow ( z, 3 ) + constant;
				re_z = fabs ( z.real () );
				im_z = fabs ( z.imag () );
			}
			//закрашиваем z, попавшие в область, случайными значениями цвета
			if ( re_z < MAX || im_z < MAX * 50 ) {
				new_clr[0] = rand() % 255;
				new_clr[1] = rand() % 255;
				new_clr[2] = rand() % 255;
				draw_dot ( x, y, new_clr );
			}
			else //точки, не попавшие в область, закрашиваем цветом фона
				draw_dot( x, y, clr_black );
		}
	}
	glFlush(); //отправляем весь вывод на экран
}
 
void init () {
	glClearColor ( 0.0, 0.0, 0.0, 0.0 ); //цвет фона(RGBA, чёрный)
	glPointSize ( 4.0 ); //точка размером 4x4 пикселя
	glMatrixMode ( GL_PROJECTION ); //матрица проекций
	glLoadIdentity (); //замена текущей матрицы на единичную
	//установка системы координат
	gluOrtho2D ( -WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2 );
}
 
void display() {
	glClear ( GL_COLOR_BUFFER_BIT ); //очистка текущего буфера цвета
	glFlush ();
	draw_biomorph (); //рисование биоморфа
}
 
int main ( int argc, char** argv ) {
	glutInit ( &argc, argv ); //инициализация OpenGL Utility Toolkit
	//режим окна(одиночный буфер и RGB-палитра)
	glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB );
	//установка размеров окна(ширина и высота в пикселях)
	glutInitWindowSize ( WIDTH, HEIGHT );
	//положение окна относительно левого верхнего угла экрана
	glutInitWindowPosition (50, 50);
	//создание окна c заголовком Biomorph
	glutCreateWindow ("Biomorph");
	//инициализация функции, отвечающей за рисование в окне
	glutDisplayFunc ( display );
	init (); // дополнительные параметры
	glutMainLoop (); //вход в главный цикл
	return 0;
}
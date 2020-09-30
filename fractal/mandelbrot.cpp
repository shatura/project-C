// Реализация C ++ для фракталов множества Мандельброта
#include <graphics.h>
#include <stdio.h>
#define MAXCOUNT 30

  
// Функция для рисования множества Мандельброта

void fractal(float left, float top, float xside, float yside)

{
    float xscale, yscale, zx, zy, cx, tempx, cy;
    int x, y, i, j;
    int maxx, maxy, count;

    // получение максимального значения оси X экрана
    maxx = getmaxx();
    // получаем максимальное значение оси Y экрана
    maxy = getmaxy();
    // настройка xscale и yscale
    xscale = xside / maxx;
    yscale = yside / maxy;

    // вызов функции прямоугольника
    // где будет видно требуемое изображение

    rectangle(0, 0, maxx, maxy);
  
    // сканируем каждую точку в этой прямоугольной области.
    // Каждая точка представляет комплексное число (x + yi).
    // Повторяем это комплексное число

    for (y = 1; y <= maxy - 1; y++) {

        for (x = 1; x <= maxx - 1; x++)

        {
            // c_real
            cx = x * xscale + left;
            // c_imaginary
            cy = y * yscale + top;
            // z_real
            zx = 0;
            // z_imaginary
            zy = 0;
            count = 0;

            // Рассчитать, принадлежит ли c (c_real + c_imaginary)
            // к множеству Мандельброта или нет и рисуем пиксель
            // в координатах (x, y) соответственно
            // Если вы достигли максимального количества итераций
            // и если расстояние от источника
            // больше 2 выход из цикла

            while ((zx * zx + zy * zy < 4) && (count < MAXCOUNT))

            {

                // Рассчитать функцию Мандельброта

                // z = z * z + c, где z - комплексное число

                // tempx = z_real * _real - z_imaginary * z_imaginary + c_real

                tempx = zx * zx - zy * zy + cx;

                // 2 * z_real * z_imaginary + c_imaginary

                zy = 2 * zx * zy + cy;

                // Обновление z_real = tempx

                zx = tempx;

                // Увеличение количества

                count = count + 1;
            }
            // Для отображения созданного фрактала
            putpixel(x, y, count);
        }
    }
}

  
// Код драйвера
int main()

{
    // gm это графический режим, который режим отображения компьютера, который
    // генерируем изображение с использованием пикселей.
    // DETECT - это макрос, определенный в заголовочный файл graphics.h

    int gd = DETECT, gm, errorcode;

    float left, top, xside, yside;

    // устанавливаем левый, верхний, xside и yside
    // для экрана и изображения для отображения

    left = -1.75;
    top = -0.25;
    xside = 0.25;
    yside = 0.45;

    char driver[] = "";

    // initgraph инициализирует графическая система путем загрузки графический драйвер с диска
    initgraph(&gd, &gm, driver);
    // вызов функции
    fractal(left, top, xside, yside);
    getch();
    // функция closegraph закрывает графический режим и освобождает
    // вся память выделена графическая система
    closegraph();
    return 0;

}
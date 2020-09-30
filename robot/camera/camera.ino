#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/twi.h> 
#include <util/delay.h>
#include <avr/pgmspace.h>

// Дефайны портов
#define CE_PIN 9
#define CSN_PIN 10
#define XCLK  11  // PB3
#define VSYNC 3   // PD3
#define PCLK  2   // PD2
#define D0    A0  // PC0
#define D1    A1  // PC1
#define D2    A2  // PC2
#define D3    A3  // PC3
#define D4    4   // PD4
#define D5    5   // PD5
#define D6    6   // PD6
#define D7    7   // PD7

RF24 radio(CE_PIN, CSN_PIN);                 //Выставляем пины на модель NRF2401+
const uint32_t pipe1 = 0xE7E7E7E7E2LL;
extern unsigned int video[0x800];           // массив для изображения

// Дефайны OV7670

#define vga         0
#define qvga        1
#define qqvga       2
#define yuv422      0
#define rgb565      1
#define bayerRGB    2
#define camAddr_WR  0x42  // Адрес камеры на запись
#define camAddr_RD  0x43  // Адрес камеры на чтение

// Регистры OV7670 (дефайны адресов регистров)

#define REG_GAIN          0x00         /* Усиление 8 бит (остальное в vref) */
#define REG_BLUE          0x01         /* усиление синего сигнала */
#define REG_RED           0x02         /* усиление красного сигнала */
#define REG_VREF          0x03         /* Pieces of GAIN, VSTART, VSTOP */
#define REG_COM1          0x04         /* управление порта */
#define COM1_CCIR656      0x40         /*  подключение протокола CCIR656 определяла последовательные и параллельные интерфейсы телевизионного студийного оборудования для передачи цифрового компонентного видеосигнала уровня 4:2:2  */
#define REG_BAVE          0x05         /* средний уровень регистра U/B ... */
#define REG_GbAVE         0x06         /* средний уровень регистра  Y/Gb для авторегуляции подсцевтки  */
#define REG_AECHH         0x07         /* система подавление Эхо на 5  бит */
#define REG_RAVE          0x08         /* средний уровень регистра V/R ... */
#define REG_COM2          0x09         /* управление порта */
#define COM2_SSLEEP       0x10         /* программа вхождение в спящий режим */
#define REG_PID           0x0a         /* хранение старшего бита информации (MSB) - стенография */
#define REG_VER           0x0b         /* хранение младшего бита информации (LSB) - стенография */
#define REG_COM3          0x0c         /* управление порта */
#define COM3_SWAP         0x40         /* регистр смены байтов  */
#define COM3_SCALEEN      0x08         /* включение масштабирование изображение  */
#define COM3_DCWEN        0x04         /* Enable downsamp/crop/window */
#define REG_COM4          0x0d         /* управление порта */
#define REG_COM5          0x0e         /* включение резервации битов памяти */
#define REG_COM6          0x0f         /* управление порта */
#define REG_AECH          0x10         /* Увеличение значение AEC -Assam Engineering College - система подавление Эхо */
#define REG_CLKRC         0x11         /* управление временем */
#define CLK_EXT           0x40         /* Прямое использование внешних часов */
#define CLK_SCALE         0x3f         /* испольхование часов внутри системы */
#define REG_COM7          0x12         /* управление порта */ 
#define COM7_RESET        0x80         /* перезагрузка регистров */
#define COM7_FMT_MASK     0x38
#define COM7_FMT_VGA      0x00
#define COM7_FMT_CIF      0x20         /* CIF (352 х 240) format */
#define COM7_FMT_QVGA     0x10         /* QVGA (320 х 240) format */
#define COM7_FMT_QCIF     0x08         /* QCIF (176 × 144) format */
#define COM7_RGB          0x04         /* бит 0 и 2 - RGB format */
#define COM7_YUV          0x00         /* YUV цветовая модель, в которой цвет представляется как 3 компоненты — яркость (Y) и две цветоразностных (U и V) */
#define COM7_BAYER        0x01         /* Bayer format - двумерный массив цветных фильтров */
#define COM7_PBAYER       0x05         /* "обработка Bayer format" */
#define REG_COM8          0x13         /* управление порта */
#define COM8_FASTAEC      0x80         /* быстрое переключение AGC(автоматическое регулировка усиление сигнала)/AEC(система подавление эхо) */
#define COM8_AECSTEP      0x40         /* неограниченный размера AEC(система подавление эхо)  */
#define COM8_BFILT        0x20         /* Разрешение полосового фильтра */
#define COM8_AGC          0x04         /* автоматическое усиление */
#define COM8_AWB          0x02         /* разрешение белого баланса */
#define COM8_AEC          0x01         /* Автоматическое определение яркости экспозиции ккартинки  */
#define REG_COM9          0x14         /* управление порта - поток усиление сигнала */
#define REG_COM10         0x15         /* управление порта */
#define COM10_HSYNC       0x40         /* HSYNC замена на HREF (импульсная строчная синхронизация) */
#define COM10_PCLK_HB     0x20         /* подавление PCLK (тактовый импульс выдачи байта с параллельного порта  D7 — D0) в горизонтальных пикселях */
#define COM10_HREF_REV    0x08         /* инверсия HREF */
#define COM10_VS_LEAD     0x04         /* VSYNC (режим 30 к/сек )  */
#define COM10_VS_NEG      0x02         /* VSYNC в негативе */
#define COM10_HS_NEG      0x01         /* HSYNC в негативе */
#define REG_HSTART        0x17         /* запуск старшего бита */
#define REG_HSTOP         0x18         /* остановка старшего бита */
#define REG_VSTART        0x19         /* вертикальный старт битов */
#define REG_VSTOP         0x1a         /* вертикальный остановка битов */
#define REG_PSHFT         0x1b         /* пиксельная задержка после HREF */
#define REG_MIDH          0x1c         /* воспроизведение высоких сигналов */
#define REG_MIDL          0x1d         /* воспроизведение низких сигналов  */
#define REG_MVFP          0x1e         /* зеркальное отражение */
#define MVFP_MIRROR       0x20         /* зеркальное изображение */
#define MVFP_FLIP         0x10         /* вертикальное флип */

#define REG_AEW           0x24        /* AGC(автоматическое регулировка усиление сигнала) верхний предел */
#define REG_AEB           0x25        /* AGC нижний предел */
#define REG_VPT           0x26        /* AGC/AEC быстрое переключение режимов */
#define REG_HSYST         0x30        /* HSYNC(кадровая обработка 30 к/сек) задержка переднего фронта */
#define REG_HSYEN         0x31        /* HSYNC задержка заднего фронта */
#define REG_HREF          0x32        /* HREF(импульс строчной синхронизации) разрыв */
#define REG_TSLB          0x3a        /* многокадровость */
#define TSLB_YLAST        0x04        /* UYVY or VYUY выбор реэжима светопередачи от  YUV цветовая модель, в которой цвет представляется как 3 компоненты — яркость (Y) и две цветоразностных (U и V) */
#define REG_COM11         0x3b        /* управление порта */
#define COM11_NIGHT       0x80        /* разрешение работы с режимом 11 порта */
#define COM11_NMFR        0x60        /* двухмерная частота кадров */
#define COM11_HZAUTO      0x10        /* автоматический выбор режима работы на частоте 50/60 Гц */
#define COM11_50HZ        0x08        /* ручной выбор на 50 Гц*/
#define COM11_EXP         0x02
#define REG_COM12         0x3c        /* управление порта 12 */
#define COM12_HREF        0x80        /* HREF всегда */
#define REG_COM13         0x3d        /* управление порта 13 */
#define COM13_GAMMA       0x80        /* включение гамма */
#define COM13_UVSAT       0x40        /* регулировка насыщенности */
#define COM13_UVSWAP      0x01        /* V before U - w/TSLB */
#define REG_COM14         0x3e        /* управление порта 14 */
#define COM14_DCWEN       0x10        /* PCLK включить */
#define REG_EDGE          0x3f        /* коэфициент усиления */
#define REG_COM15         0x40        /* управление порта 15 */
#define COM15_R10F0       0x00        /* диапозон данных 10 to F0 */
#define COM15_R01FE       0x80        /* диапозон данных 01 to FE */
#define COM15_R00FF       0xc0        /* диапозон данных 00 to FF */
#define COM15_RGB565      0x10        /* RGB565 выход */
#define COM15_RGB555      0x30        /* RGB555 выход */
#define REG_COM16         0x41        /* управление порта 16 */
#define COM16_AWBGAIN     0x08        /* AWB (стандарт многополосный широкоимпульсный адаптер) кодировка  */
#define REG_COM17         0x42        /* управление порта 17 */
#define COM17_AECWIN      0xc0        /* AEC система подавление эхо на  COM4 */
#define COM17_CBAR        0x08        /* DSP сигнальный ЦП для оформление цвета */
/*
   Эта матрица определяет, как будут генерироваться цвета.
   Настраивается для настройки оттенка и насыщенности.
   Заказ: v-красный, v-зеленый, v-синий, u-красный, u-зеленый, u-синий
   Это девятибитные знаковые числа со знаком
   Хранится в 0x58.Sign для v-красного - это бит 0, и вверх оттуда.
*/
#define REG_CMATRIX_BASE         0x4f
#define CMATRIX_LEN               6
#define REG_CMATRIX_SIGN         0x58
#define REG_BRIGHT               0x55  /* Яркость */
#define REG_CONTRAS              0x56  /* контрастность */
#define REG_GFIX                 0x69  /* контроль усиления фиксации изображения */
#define REG_REG76                0x76  /* определение видеокамеры в названиях изображения */
#define R76_BLKPCOR              0x80  /* Разрешение коррекции черных пикселей */
#define R76_WHTPCOR              0x40  /* Разрешение коррекции белых пикселей */
#define REG_RGB444               0x8c  /* RGB 444  */
#define R444_ENABLE              0x02  /* включить RGB444 */
#define R444_RGBX                0x01  /* Пустой полубайт в конце */
#define REG_HAECC1               0x9f  /* Хранение AEC/AGC в порте 1 */
#define REG_HAECC2               0xa0  /* Хранение AEC/AGC в порте2 */
#define REG_BD50MAX              0xa5  /* 50hz предел шага обвязки */
#define REG_HAECC3               0xa6  /* Хранение AEC/AGC в порте 3 */
#define REG_HAECC4               0xa7  /* Хранение AEC/AGC в порте 4 */
#define REG_HAECC5               0xa8  /* Хранение AEC/AGC в порте 5 */
#define REG_HAECC6               0xa9  /* Хранение AEC/AGC в порте */
#define REG_HAECC7               0xaa  /* Хранение AEC/AGC в порте */
#define REG_BD60MAX              0xab  /* 60hz предел шага обвязки */

#define MTX1                     0x4f  /* Матричный коэффициент 1 */
#define MTX2                     0x50  /* Матричный коэффициент 2 */
#define MTX3                     0x51  /* Матричный коэффициент 3 */
#define MTX4                     0x52  /* Матричный коэффициент 4 */
#define MTX5                     0x53  /* Матричный коэффициент 5 */
#define MTX6                     0x54  /* Матричный коэффициент 6 */
#define MTXS                     0x58  /* Матричный коэффициент Сигнала */
#define AWBC7                    0x59  /* AWB(Адаптивная Многоскоростная Широкополосный) порта 7 */
#define AWBC8                    0x5a  /* AWB (Адаптивная Многоскоростная Широкополосный) порта  8 */
#define AWBC9                    0x5b  /* AWB (Адаптивная Многоскоростная Широкополосный) порта  9 */
#define AWBC10                   0x5c  /* AWB (Адаптивная Многоскоростная Широкополосный) порта  10 */
#define AWBC11                   0x5d  /* AWB (Адаптивная Многоскоростная Широкополосный) порта  11 */
#define AWBC12                   0x5e  /* AWB (Адаптивная Многоскоростная Широкополосный) порта  12 */
#define GGAIN                    0x6a  /* фиксированное изменение AWB */
#define DBLV                     0x6b
#define AWBCTR3                  0x6c  /* AWB (Адаптивная Многоскоростная Широкополосный) порта  3 */
#define AWBCTR2                  0x6d  /* AWB (Адаптивная Многоскоростная Широкополосный) порта  2 */
#define AWBCTR1                  0x6e  /* AWB (Адаптивная Многоскоростная Широкополосный) порта  1 */
#define AWBCTR0                  0x6f  /* AWB (Адаптивная Многоскоростная Широкополосный) порта  0 */

#define SCAXLSIC_XSC      0x70  //
#define SCAXLSIC_YSC      0x71  //
#define DNSTH             0x4c  // Сила снижения шума
#define REG77             0x77  // Смещение шума

#define SCALING_DCWCTR    0x72
#define SCALING_PCLK_DIV  0x73

////////////////////////////////////////////////////СТРУКТУРЫ////////////////////////////////////////////////////
struct RegVal{
  uint8_t reg_num;       // Адресс регистра
  uint16_t value;        // Значение регистра
};
//---------------------------------------------------------------------------------
const struct RegVal QVGA_OV7670[] PROGMEM = { // настройка разрешение 320 на 240
  { REG_COM14,        0x19 },
  { SCALING_DCWCTR,   0x11 },
  { SCALING_PCLK_DIV, 0xf1 },
  { REG_HSTART,       0x16 },
  { REG_HSTOP,        0x04 },
  { REG_HREF,         0xa4 },
  { REG_VSTART,       0x02 },
  { REG_VSTOP,        0x7a },
  { REG_VREF,         0x0a },
  { 0xFF,             0xff },         
};
//---------------------------------------------------------------------------------
const struct RegVal YUV422_OV7670[] PROGMEM = {   //настройка режима цвета
  { REG_COM7, 0x0 },    /* выбор режима YUV */
  { REG_RGB444, 0 },    /* отрицание режима работы РГБ444 */
  { REG_COM1, 0 },
  { REG_COM15, COM15_R00FF },
  { REG_COM9, 0x6A },   /* 128x усиления потока; 0x8 is резервация бита */
  { 0x4f, 0x80 },       /* "Коэффициент матрицы 1" */
  { 0x50, 0x80 },       /* "Коэффициент матрицы 2" */
  { 0x51, 0 },          /* vb */
  { 0x52, 0x22 },       /* "Коэффициент матрицы 4" */
  { 0x53, 0x5e },       /* "Коэффициент матрицы 5" */
  { 0x54, 0x80 },       /* "Коэффициент матрицы 6" */
  { REG_COM13, COM13_UVSAT },
  { 0xff, 0xff },       
};
//---------------------------------------------------------------------------------
const struct RegVal OV7670_default_regs[] PROGMEM = { //регистры по умолчанию
  { REG_COM7, COM7_RESET },
  { REG_TSLB, 0x04 },       /* OV */
  { REG_COM7, 0 },          
  
  //Установить аппаратное окно связи
  { REG_HSTART, 0x13 }, { REG_HSTOP, 0x01 },
  { REG_HREF, 0xb6 }, { REG_VSTART, 0x02 },
  { REG_VSTOP, 0x7a }, { REG_VREF, 0x0a },
  { REG_COM3, 0 }, { REG_COM14, 0 },
  
  /* мастабирование */
  { SCAXLSIC_XSC, 0x3a }, { SCAXLSIC_YSC, 0x35 },
  //{ SCAXLSIC_XSC, 0xBA }, { SCAXLSIC_YSC, 0xB5 }, //sps: Включение настроечной таблицы
  { 0x72, 0x11 }, { 0x73,       0xf0 },
  { 0xa2, 0x01 }, { REG_COM10,  0x0 },
  
  /* гамма настройка цветов */
  { 0x7a, 0x20 }, { 0x7b, 0x10 },
  { 0x7c, 0x1e }, { 0x7d, 0x35 },
  { 0x7e, 0x5a }, { 0x7f, 0x69 },
  { 0x80, 0x76 }, { 0x81, 0x80 },
  { 0x82, 0x88 }, { 0x83, 0x8f },
  { 0x84, 0x96 }, { 0x85, 0xa3 },
  { 0x86, 0xaf }, { 0x87, 0xc4 },
  { 0x88, 0xd7 }, { 0x89, 0xe8 },
  
  /* Параметры AGC(автоматическое регулировка усиление сигнала) и AEC(аккустическое подавление эхо). Обратите внимание, что мы начинаем с отключения этих функций,
     Затем включите их только после настройки значений. */
  { REG_COM8, COM8_FASTAEC | COM8_AECSTEP },
  { REG_GAIN, 0 }, { REG_AECH, 0 },
  { REG_COM4, 0x40 },                           /* резервация бита */
  { REG_COM9, 0x18 },                           /* 4x получить + резервация */
  { REG_BD50MAX, 0x05 },{ REG_BD60MAX, 0x07 },
  { REG_AEW, 0x95 },    { REG_AEB, 0x33 },
  { REG_VPT, 0xe3 },    { REG_HAECC1, 0x78 },
  { REG_HAECC2, 0x68 }, { 0xa1, 0x03 },         
  { REG_HAECC3, 0xd8 }, { REG_HAECC4, 0xd8 },
  { REG_HAECC5, 0xf0 }, { REG_HAECC6, 0x90 },
  { REG_HAECC7, 0x94 },
  { REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_AGC | COM8_AEC },
  { 0x30, 0 }, { 0x31, 0 },
  
  /* Резервация значения  */
  { REG_COM5, 0x61 }, { REG_COM6, 0x4b },
  { 0x16, 0x02 }, { REG_MVFP, 0x07 },
  { 0x21, 0x02 }, { 0x22, 0x91 },
  { 0x29, 0x07 }, { 0x33, 0x0b },
  { 0x35, 0x0b }, { 0x37, 0x1d },
  { 0x38, 0x71 }, { 0x39, 0x2a },
  { REG_COM12, 0x78 }, { 0x4d, 0x40 },
  { 0x4e, 0x20 }, { REG_GFIX, 0 },
  /*{0x6b, 0x4a}*/{ 0x74, 0x10 },
  { 0x8d, 0x4f }, { 0x8e, 0 },
  { 0x8f, 0 },    { 0x90, 0 },
  { 0x91, 0 },    { 0x96, 0 },
  { 0x9a, 0 },    { 0xb0, 0x84 },
  { 0xb1, 0x0c }, { 0xb2, 0x0e },
  { 0xb3, 0x82 }, { 0xb8, 0x0a },

  /* Резервация и настройка белого баланса */
  { 0x43, 0x0a }, { 0x44, 0xf0 },
  { 0x45, 0x34 }, { 0x46, 0x58 },
  { 0x47, 0x28 }, { 0x48, 0x3a },
  { 0x59, 0x88 }, { 0x5a, 0x88 },
  { 0x5b, 0x44 }, { 0x5c, 0x67 },
  { 0x5d, 0x49 }, { 0x5e, 0x0e },
  { 0x6c, 0x0a }, { 0x6d, 0x55 },
  { 0x6e, 0x11 }, { 0x6f, 0x9e }, 
  { GGAIN, 0x40 }, { REG_BLUE, 0x40 },
  { REG_RED, 0x60 },
  { REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_AGC | COM8_AEC | COM8_AWB },

  /* Матричные коэффициенты  */
  { 0x4f, 0x80 }, { 0x50, 0x80 },
  { 0x51, 0 },    { 0x52, 0x22 },
  { 0x53, 0x5e }, { 0x54, 0x80 },
  { 0x58, 0x9e },

  { REG_COM16, COM16_AWBGAIN }, { REG_EDGE, 0xF }, //sps установил EDGE-> 0xF
  { 0x75, 0x05 }, { REG_REG76, 0xe1 },
  { DNSTH, 0 },{ REG77, 0x01 },
 // { DNSTH, 0x5 },    { REG77, 0x10 },  // sps exp's     
  { REG_COM13, /*0xc3*/0x48 }, { 0x4b, 0x09 },
  { 0xc9, 0x60 },   /*{REG_COM16, 0x38},*/
  { 0x56, 0x40 },

  { 0x34, 0x11 }, { REG_COM11, COM11_EXP | COM11_HZAUTO },
  { 0xa4, 0x82/*был 0x88*/ }, { 0x96, 0 },
  { 0x97, 0x30 }, { 0x98, 0x20 },
  { 0x99, 0x30 }, { 0x9a, 0x84 },
  { 0x9b, 0x29 }, { 0x9c, 0x03 },
  { 0x9d, 0x4c }, { 0x9e, 0x3f },
  { 0x78, 0x04 },

  /* мультиплексорный регистр */
  { 0x79, 0x01 }, { 0xc8, 0xf0 },
  { 0x79, 0x0f }, { 0xc8, 0x00 },
  { 0x79, 0x10 }, { 0xc8, 0x7e },
  { 0x79, 0x0a }, { 0xc8, 0x80 },
  { 0x79, 0x0b }, { 0xc8, 0x01 },
  { 0x79, 0x0c }, { 0xc8, 0x0f },
  { 0x79, 0x0d }, { 0xc8, 0x20 },
  { 0x79, 0x09 }, { 0xc8, 0x80 },
  { 0x79, 0x02 }, { 0xc8, 0xc0 },
  { 0x79, 0x03 }, { 0xc8, 0x40 },
  { 0x79, 0x05 }, { 0xc8, 0x30 },
  { 0x79, 0x26 },
  { 0xff, 0xff }, /* END MARKER */
};
///////////////////////////////////////// ИНИЦИАЛИЗАЦИЯ///////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);   // Настраиваем последовательный порт (768000)
  HrdwareInit();          // Настраиваем периферию
  camInit();              // Стартовая инициализация камеры
  setResolution();        // Выставляем разрешение
  setColor();             // Выставляем формат цвета
  wrReg(REG_CLKRC, 31);   // Выставляем предделитель , замена в случае не контакта wrReg(0x11, 12)
  radio.begin(); //подключение радио
  //radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  // radio.setPayloadSize(32);     //размер пакета, в байтах
  radio.setDataRate(RF24_2MBPS);             // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  //radio.setPALevel(RF24_PA_MAX);             // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.openWritingPipe(pipe1);               //truba
  //radio.setChannel(100);  //выбираем канал (в котором нет шумов!)           
}
/////////////////////////////////////////////ОСНОВНЫЙ ЦИКЛ///////////////////////////////////////////////
void loop() {
  captureImg(96, 69);   // Считываем и пересылаем кадр 
  radio.write( video, sizeof(video) );
}
//////////////////////////////////////////////////////ФУНКЦИИ/////////////////////////////////////////////////////
void HrdwareInit(void) {
  cli();  // Отключаем все прерывания перед началом инициализации
  
  // Настраиваем генератор тактового сигнала на 8МГц
  DDRB = 0x00;                                                                 // Чистим порт от предустановок. Актуально для Arduino Nano v3
  DDRB |= _BV(PB3);                                                            // Для генерации сигнала XCLK используем пин PB3 (#11 Arduino Nano v3)
  ASSR &= ~(1 << EXCLK | 1 << AS2);                                            // Частота с внешнего кварца / асинхронный таймер
  TCCR2A = (1 << COM2A0) | (1 << WGM21) | (1 << WGM20);                        // выход в [1] при совпадении / режим Fast PWM / режим Fast PWM
  TCCR2B = (1 << WGM22) | (1 << CS20);                                         // режим Fast PWM / без предделителя
  OCR2A = 0;                                                                   // [F_CPU/2]
  DDRC &= ~(1 << PC0 | 1 << PC1 | 1 << PC2 | 1 << PC3);                        // Входы для парралельного порта камеры (Arduino #D0-D3)
  DDRD &= ~(1 << PD2 | 1 << PD3 | 1 << PD4 | 1 << PD5 | 1 << PD6 | 1 << PD7);  // Входы для парралельного порта камеры и входы синхронизации (Arduino #D4-D7)
  _delay_ms(3000);

  // Настройка TWI
  TWSR &= ~(1 << TWPS0 | 1 << TWPS1);                                          //Отключаем предделитель для TWI (Предделитель = 1)
  TWBR = 72;                                                                   //Настройка TWI на 100кГц [F_CPU/(16+2*TWBR*Prescaler)]
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void camInit(void){
  wrReg(REG_COM7, 0x80);                                // Сброс регистров SCCB
  _delay_ms(100);                                       // Задержка в 100 мс.
  wrSensorRegs(OV7670_default_regs);
  wrReg(REG_COM10, 32);                                 // PCLK не переключается во время HBLANK
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setResolution(void){
  wrReg(REG_COM3, 4);                                   // Разрешаем масштабирование
  wrSensorRegs(QVGA_OV7670);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setColor(void){
  wrSensorRegs(YUV422_OV7670);                          // Устанавливаем цветовое пространство YUV (Формат 422)
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void wrSensorRegs(const struct RegVal reglist[]){
  uint8_t reg_addr, reg_val;
  const struct RegVal *next = reglist;
  while ((reg_addr != 0xff) | (reg_val != 0xff)){
    reg_addr = pgm_read_byte(&next->reg_num);
    reg_val = pgm_read_byte(&next->value);
    wrReg(reg_addr, reg_val);
    next++;
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////static void captureImg(uint16_t wg, uint16_t hg){
  uint16_t y, x;
  byte data;
 

  Serial.write(255);              // Инициируем начало кадра 

  // while (!(PIND & 8));         //  ждкм высокий сигнал с PD3
  // while ((PIND & 8));          //  ждем низкий сигнал
  while (!(digitalRead(VSYNC)));  // Ждем установки [1] на VSYNC
  while ((digitalRead(VSYNC)));   // Ждем установки [0] на VSYNC

  y = hg;
  while (y--){
        x = wg;
      //while (!(PIND & 256)); //ждем высокий сигнал
    while (x--){
      //while ((PIND & 4)); //ждем низкий сигнал
      
      while ((digitalRead(PCLK)));   // Ждем установки [0] на PCLK
//-----------------------------------------------------------[Обрабатываем и отсылаем полу-ченный пиксель] 
      data = (PINC & 15) | (PIND & 240); // Получаем данные с виртуального порта (D0-D7) через физ.пины [!]
      if(data==255)data=254;             // Подгоняем данные под формат пересылки
      Serial.write(data);                // Шлем байт
     // Serial.write(random(250));
//-----------------------------------------------------------     
      //while (!(PIND & 4));  //ожидание высокого сигнала
      //while ((PIND & 4));   //ожидание низкого сигнала
      //while (!(PIND & 4));  //ожидание высокого сигнала
    //  for(byte pass; pass<32;pass++)
   //   {
        while (!(digitalRead(PCLK)));  // Ждем установки [1] на PCLK
        while ((digitalRead(PCLK)));   // Ждем установки [0] на PCLK
        while (!(digitalRead(PCLK)));  // Ждем установки [1] на PCLK
   //   }
      
    }
    //  while ((PIND & 256)); /ожидание низкого сигнала
  }
    _delay_ms(100);
}
//////////////////////////////////////// ФУНКЦИИ РАБОТЫ С I2C ///////////////////////////////////////

//-----------------------------------------------------------[ Начало передачи ]
void twiStart(void){
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);          //начать отправление
  while (!(TWCR & (1 << TWINT)));                      //Ожидание начала передачи
  if ((TWSR & 0xF8) != TW_START)
    error();
}
//-----------------------------------------------------------[ Передать байт ]
void twiWriteByte(uint8_t DATA, uint8_t type){
  TWDR = DATA;
  TWCR = _BV(TWINT) | _BV(TWEN);
  while (!(TWCR & (1 << TWINT))) {}
  if ((TWSR & 0xF8) != type)
    error();;
}
//-----------------------------------------------------------[ Передать адрес ]
void twiAddr(uint8_t addr, uint8_t typeTWI){
  TWDR = addr;                                          // Отправить адрес
  TWCR = _BV(TWINT) | _BV(TWEN);                        // Четкое прерывание для начала передачи
  while ((TWCR & _BV(TWINT)) == 0);                     // начать отправку */
  if ((TWSR & 0xF8) != typeTWI)
    error();
}
//-----------------------------------------------------------[ Передать регистр ]
void wrReg(uint8_t reg, uint8_t dat){
  //send start condition
  twiStart();
  twiAddr(camAddr_WR, TW_MT_SLA_ACK);
  twiWriteByte(reg, TW_MT_DATA_ACK);
  twiWriteByte(dat, TW_MT_DATA_ACK);
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);     //Остановить отправку
  _delay_ms(1);
}
//-----------------------------------------------------------[ Принять байт ]
static uint8_t twiRd(uint8_t nack){
  if (nack){
    TWCR = _BV(TWINT) | _BV(TWEN);
    while ((TWCR & _BV(TWINT)) == 0);                   // начать отправку
    if ((TWSR & 0xF8) != TW_MR_DATA_NACK)
      error();
    return TWDR;
  }
  else{
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
    while ((TWCR & _BV(TWINT)) == 0);                   // начать отправку
    if ((TWSR & 0xF8) != TW_MR_DATA_ACK)
      error();
    return TWDR;
  }
}
//-----------------------------------------------------------[ Принять регистр ]
uint8_t rdReg(uint8_t reg){
  uint8_t dat;
  twiStart();
  twiAddr(camAddr_WR, TW_MT_SLA_ACK);
  twiWriteByte(reg, TW_MT_DATA_ACK);
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);     //Остановить отправку
  _delay_ms(1);
  twiStart();
  twiAddr(camAddr_RD, TW_MR_SLA_ACK);
  dat = twiRd(1);
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);     //Остановить отправку
  _delay_ms(1);
  return dat;
}
//-----------------------------------------------------------[ Ошибка ]
void error(void){
  }

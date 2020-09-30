#include <nRF24L01.h>	//инициализация NRF24
#include <RF24.h>		//инициализация NRF24
#include <RF24_config.h>	//программная конфигурация NRF24
#include <SPI.h>		//подключение порта
#include <UTFT.h>		//библиотека подключение экрана
#define CE_PIN   10	//подлючение NRF CE
#define CSN_PIN 9		//подлючение NRF CSN
#define JOYSTICK_X A0	//объявление портов управления
#define JOYSTICK_Y A1	//объявление портов управления
#define JOYSTICK_Z A2		//объявление портов управления
const uint64_t pipe = 0xE7E7E7E7E1LL;
const uint64_t pipe1 = 0xE7E7E7E7E2LL;
UTFT myDisplay (CTE32HR, 38, 39, 40, 41);    // и передаём идентификатор модели дисплея и номера управляющих пинов
int analog[3]; // массив из 2-х элементов, в котором хранятся с джойстика данные
byte latest_analog[3]; // массив, хранящий последние переданные данные
boolean flag; // флажок отправки данных
extern unsigned int video[0x800]; // массив для изображения
RF24 radio(CE_PIN, CSN_PIN);  //Выставляем пины на модель NRF2401+
void setup() {   
Serial.begin(9600); 		//подключение последовательного порта
myDisplay .InitLCD() ;     // инициализируем дисплей с горизонтальной ориентацией
myDisplay .clrScr();      // очищаем экран
radio.begin();        //подключение радио
radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
radio.setPayloadSize(32);     //размер пакета, в байтах
radio.setDataRate(RF24_2MBPS);             // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
radio.setPALevel(RF24_PA_MAX);             // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
radio.openWritingPipe(pipe);               //запись в трубу
radio.openReadingPipe(1, pipe1);           //чтение с трубы
radio.setChannel(100);  //выбираем канал (в котором нет шумов!) ot 0 do 127
}
void loop() {
// посылка
Serial.print(" X: ");		//вывод значение в последовательный порт
Serial.print(analog[0]); 	//вывод значение в последовательный порт
Serial.print(", Y: ");		//вывод значение в последовательный порт
Serial.print(analog[1]); 	//вывод значение в последовательный порт
Serial.print(", Z: ");		//вывод значение в последовательный порт
Serial.println(analog[2]);	 //вывод значение в последовательный порт

analog[0] = map(analogRead(JOYSTICK_X), 0, 1023, 0, 253);  //преобразование значений
analog[1] = map(analogRead(JOYSTICK_Y), 0, 1023, 0, 253);  //преобразование значений
analog[2] = map(analogRead(JOYSTICK_Z), 0, 1023, 0, 180);  //преобразование значений

for (int i = 0; i < 3; i++) { // в цикле от 0 до числа каналов
if (analog[i] != latest_analog[i]) { // если есть изменения в analog
flag = 1; // поднять флаг отправки по радио
latest_analog[i] = analog[i]; // запомнить последнее изменение
}}
if (flag == 1) {
 radio.powerUp(); // включить передатчик
radio.write(&analog, sizeof(analog)); // отправить по радио
flag = 0; //опустить флаг
radio.powerDown(); // выключить передатчик
}
//принятие
if ( radio.available() )
   {
    radio.read( video, sizeof(video) );

    // выводим изображение
    myDisplay.drawBitmap(0, 0, 96, 69, video); 	//вывод изображение координаты ,мастаб, массив
    }
  delay(1000);
   } 

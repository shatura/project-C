#include <nRF24L01.h>//инициализация NRF24
#include <RF24.h>		//инициализация NRF24
#include <RF24_config.h>	//программная конфигурация NRF
#include <Servo.h>		//инициализация серво
#include <SPI.h>		//подключение SPI портов
#define CE_PIN 8	 	//подлючение NRF CE
#define CSN_PIN 9		//подлючение NRF CSN
int ENA = 5;    // порт для ШИМ
int in1 = 4;  //логический порт для первого двигателя
int in2 = 2;  //логический порт для первого двигателя
int ENB = 3;   // порт для ШИМ
int in3 = 1;  //логический порт для второго двигателя
int in4 = 0;  //логический порт для второго двигателя 
const uint32_t pipe = 0xE7E7E7E7E1LL;		//инициализация трубы 
int analog[3];  // массив из 2-х элементов, в котором хранятся с джойстика данные
RF24 radio(CE_PIN, CSN_PIN); //Выставляем пины на модель NRF2401+
Servo myservo ; 	// подключение сервопривода
int x_pin = 0;		//объявление переменных
int y_pin = 0;		 //объявление переменных
void setup() {
Serial.begin(9600);		 //подключение последовательного порта
pinMode(ENA, OUTPUT);	//назначение на выход
pinMode(ENB, OUTPUT); 	//назначение на выход
pinMode(in1, OUTPUT); 	//назначение на выход
pinMode(in2, OUTPUT); 	//назначение на выход
pinMode(in3, OUTPUT); 	//назначение на выход
pinMode(in4, OUTPUT); 	//назначение на выход
myservo.attach(1);   // сервопривод назначенние порта
radio.begin(); //подключение радио
radio.setPayloadSize(32);     //размер пакета, в байтах
radio.setDataRate(RF24_2MBPS);  // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
radio.setPALevel(RF24_PA_MAX);  // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
radio.openReadingPipe(1, pipe);              //труба для чтение
radio.setChannel(0x127);  //выбираем канал (в котором нет шумов!)
radio.powerUp(); //начать работу
radio.startListening();   // прослушка
}
void loop() 
{
if ( radio.available() )		//начать прослушку
{radio.read(analog, sizeof(analog) );	//принятая информация
Serial.print(" X: ");		//вывод значение в последовательный порт
Serial.print(analog[0]); 	//вывод значение в последовательный порт
Serial.print(", Y: ");		//вывод значение в последовательный порт
Serial.print(analog[1]); 	//вывод значение в последовательный порт
Serial.print(", Z: ");		//вывод значение в последовательный порт
Serial.println(analog[2]);	 //вывод значение в последовательный порт
myservo.write(analog[2]); // повернуть серво на угол 0..180
x_pin=analog[0];
y_pin=analog[1];
up_a = map(x_pin,100,0, 0,255);	//ШИМ для первого двигателя
up_b = map(x_pin,150,255, 0,255); //ШИМ для второго двигателя
down_a = map(y_pin,100,0, 0,255); //ШИМ для первого двигателя
down_b = map(y_pin,150,255, 0,255) //ШИМ для второго двигателя

if (x_pin <= 100)		//значение х меньше или равно 100
{
digitalWrite(ENA, HIGH); // включаем канал A
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW);
}
else if (x_pin > 101 && x_pin < 149)
{
digitalWrite(in1, LOW);
digitalWrite(in2, LOW);
}
if (x_pin >= 150) 		//значение х больше или равно 150
{
digitalWrite(ENA, HIGH); // включаем канал A
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);
}
else if (x_pin > 101 && x_pin < 149)
{
digitalWrite(in1, LOW);
digitalWrite(in2, LOW);
}
if (y_pin <= 100)		//значение у меньше или равно 100
{
digitalWrite(ENB, HIGH); // включаем канал В
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW);
}
else if (y_pin > 101 && y_pin < 149)
{
digitalWrite(in3, LOW);
digitalWrite(in4, LOW);
}
if (y_pin >= 150) 		//значение у больше или равно 150
{
digitalWrite(ENB, HIGH);  // включаем канал B
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
}
else if (y_pin > 101 && y_pin < 149)
{
digitalWrite(in3, LOW);
digitalWrite(in4, LOW);
}}} 

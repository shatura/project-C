    #include <WiFi.h>
    #include <TroykaIMU.h>
    #include <Wire.h>
    
    // множитель фильтра
    #define BETA 0.0.01745329252f              // этот коэффициент нужен нам для перевода градусов в радианы
 /*---------------------------------------------------------------------------------------------------------------------*/
    Madgwick filter;                           // создаём объект для фильтра Madgwick
    Gyroscope gyro;                            // создаём объект для работы с гироскопом
    Accelerometer accel;                       // создаём объект для работы с акселерометром

    float gx, gy, gz, ax, ay, az;              // переменные для данных с гироскопов, акселерометров
    float yaw, pitch, roll;                    // получаемые углы ориентации
    float fps = 100;                           // переменная для хранения частоты выборок фильтра
/*---------------------------------------------------------------------------------------------------------------------*/
    /*IPAddres ip (192.168.10.10);
    *DNS
    *gateway: IP-адрес сетевого шлюза (массива из 4 байт). 
    *Не обязательный параметр: по умолчанию равен IP-адресу устройства с единицами в последнем октете.
    *subnet: маска подсети (массив из 4 байт). Не обязательный параметр: по умолчанию равен 255.255.255.0
    */
    char ssid[] = "SayHand";                   //SSID-имя сети  
    char pass[] = "SayHand";                   //пароль WPA-сети
    int status = WL_IDLE_STATUS;               //если включено питание, но соединение с сетью не установлено
    char servername[]="   ";                   // удаленный сервер, с которым будет устанавливаться соединение
    WiFiServer server(80);

/*---------------------------------------------------------------------------------------------------------------------*/
    /*
    //флекс-сенсоры
    const int flexpin1 = ; подключение к аналоговым пинам через резитор в 10 кОм-47кОм
    const int flexpin2 = ; 
    const int flexpin3 = ; 
    const int flexpin4 = ; 
    const int flexpin5 = ; 
    */

void setup() {
    // открываем последовательный порт
    Serial.begin(115200);
    Serial.println("Инициализация...");
    accel.begin();                             // инициализация акселерометра
    gyro.begin();                              // инициализация гироскопа
    Serial.println("ЗБСТ ВСЁ");                // выводим сообщение об удачной инициализации гироскопа и акселерометра 
    
     // проверяем наличие платы расширения:
      if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi отсутствует"); 
        while(true);                           // дальше не продолжаем
      } 
     
     //WiFi.config(ip);
     Serial.println("Попытка подключиться к сети WPA...");
     Serial.print("SSID: ");
     Serial.println(ssid);
       
     status = WiFi.begin(ssid, pass);
     if ( status != WL_CONNECTED) { 
        Serial.println("Не удалось получить соединение Wi-Fi");
        while(true);
      } 
        else {
          server.begin();
          Serial.print("Подключен к Wi-Fi. Мой адрес:");
          IPAddress myAddress = WiFi.localIP();
          Serial.println(myAddress);
       
        }
 }








void loop() {
    
    unsigned long startMillis = millis();      // запоминаем текущее время  
    accel.readGXYZ(&ax, &ay, &az);             // считываем данные с акселерометра в единицах G
    gyro.readRadPerSecXYZ(&gx, &gy, &gz);      // считываем данные с акселерометра в радианах в секунду
    filter.setKoeff(fps, BETA);                // устанавливаем коэффициенты фильтра
    filter.update(gx, gy, gz, ax, ay, az);     // обновляем входные данные в фильтр
    yaw =  filter.getYawDeg();                 // получение углов yaw, pitch и roll из фильтра
    pitch = filter.getPitchDeg();
    roll = filter.getRollDeg();
    
    // выводим полученные углы в serial-порт
    Serial.print("высота: ");
    Serial.print(yaw);
    Serial.print("\t\t");
    Serial.print("ширина: ");
    Serial.print(pitch);
    Serial.print("\t\t");
    Serial.print("длина: ");
    Serial.println(roll);
    unsigned long deltaMillis = millis() - startMillis;// вычисляем затраченное время на обработку данных
    fps = 1000 / deltaMillis;                   // вычисляем частоту обработки фильтра
    
/*---------------------------------------------------------------------------------------------------------------------*/

    WiFiClient client = server.available();
    if (client == true)
    


}

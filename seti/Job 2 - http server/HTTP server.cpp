/*1 шаг 
создание сокета (это абстракция, которая 
позволяет работать с сетевыми ресурсами, 
как с файлами. Мы можем писать и читать
данные из сокета почти так же, как из обычного файла.)

2 шаг
привязываем ip к сокету

3 шаг
Подготовка сокета к принятию входящих соединений (listen)

4 шаг
Ожидание входящего соединения (accept)

5 шаг
Получение запроса и отправка ответа

6 шаг 
Последовательная обработка запросов
*/

#include <iostream>
#include <sstream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define _WIN32_WINNT 0x501 /*указ на минимальную систему для работы*/

#pragma comment(lib, "Ws2_32.lib") /*Необходимо, чтобы линковка(связь с файлами) 
									происходила с DLL-библиотекой для работы с сокетам */
using std::cerr;	/*поток для ошибок*/

int main()
{
	WSADATA wsaData;/*служебная структура для хранение информации
						о реализации Windows Sockets*/
	int result = WSAStartap(MAKEWORD(2, 2), &wsaData);//запуск сокета
	if (result != 0) {	// Если произошла ошибка подгрузки библиотеки
		cerr << "WSAStartup failed: " << result << "\n";
		return result;
	}
		struct addrinfo* addr = NULL; // структура, хранящая информацию об IP-адресе  слущающего сокета
		struct addrinfo hints; /* Шаблон для инициализации структуры адреса*/
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;// AF_INET определяет, что используется сеть для работы с сокетом
		hints.ai_socktype = SOCK_STREAM; // Задаем потоковый тип сокета
		hints.ai_protocol = IPPROTO_TCP; // Используем протокол TCP
		hints.ai_flags = AI_PASSIVE;    // Сокет биндится на адрес, чтобы принимать входящие соединения
		result = getaddrinfo("127.0.0.1", "8000", &hints, &addr); /* Инициализируем структуру, хранящую адрес сокета - addr.
																  HTTP-сервер будет висеть на 8000-м порту локалхоста*/

		if (result != 0) {	/*Если инициализация структуры адреса завершилась с ошибкой,
							выведем сообщением об этом и завершим выполнение программы */
			cerr << "getaddrinfo failed: " << result << "\n";
			WSACleanup(); // выгрузка библиотеки Ws2_32.dll
			return 1;
		}

		
		int listen_socket = socket(addr->ai_family, addr->ai_socktype,addr->ai_protocol);// Создание сокета
		/* Если создание сокета завершилось с ошибкой, выводим сообщение,
		освобождаем память, выделенную под структуру addr,
		выгружаем dll-библиотеку и закрываем программу*/
		if (listen_socket == INVALID_SOCKET) {
			cerr << "Error at socket: " << WSAGetLastError() << "\n";
			freeaddrinfo(addr);
			WSACleanup();
			return 1;
		}
/*--------------------------------------------------------------------------------*/
		/*привязываем к сокету адрес*/
		result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

		/*Если привязать адрес к сокету не удалось, то выводим сообщение
		 об ошибке, освобождаем память, выделенную под структуру addr.
		 и закрываем открытый сокет.
		 Выгружаем DLL-библиотеку из памяти и закрываем программу.*/
		if (result == SOCKET_ERROR) {
			cerr << "bind failed with error: " << WSAGetLastError() << "\n";
			freeaddrinfo(addr);
			closesocket(listen_socket);
			WSACleanup();
			return 1;
		}
/*--------------------------------------------------------------------------------*/
		/*Подготовка сокета к принятию входящих соединений(listen)*/
		
		// Инициализируем слушающий сокет
		if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
			cerr << "listen failed with error: " << WSAGetLastError() << "\n";
			closesocket(listen_socket);
			WSACleanup();
			return 1;
		}
/*--------------------------------------------------------------------------------*/
		/*Ожидание входящего соединения (accept)*/

		// Принимаем входящие соединения
			int client_socket = accept(listen_socket, NULL, NULL);
			if (client_socket == INVALID_SOCKET) {
				cerr << "accept failed: " << WSAGetLastError() << "\n";
				closesocket(listen_socket);
				WSACleanup();
				return 1;
			}
/*--------------------------------------------------------------------------------*/
		/*Ожидание входящего соединения (accept)*/
		// Принимаем входящие соединения
			int client_socket = accept(listen_socket, NULL, NULL);
			if (client_socket == INVALID_SOCKET) {
				cerr << "accept failed: " << WSAGetLastError() << "\n";
				closesocket(listen_socket);
				WSACleanup();
				return 1;
			}
/*--------------------------------------------------------------------------------*/
		/*Получение запроса и отправка ответа*/
			const int max_client_buffer_size = 1024;
			char buf[max_client_buffer_size];

			result = recv(client_socket, buf, max_client_buffer_size, 0);

			std::stringstream response; // сюда будет записываться ответ клиенту
			std::stringstream response_body; // тело ответа

			if (result == SOCKET_ERROR) {
				// ошибка получения данных
				cerr << "recv failed: " << result << "\n";
				closesocket(client_socket);
			}
			else if (result == 0) {
				// соединение закрыто клиентом
				cerr << "connection closed...\n";
			}
			else if (result > 0) {
				/* Мы знаем фактический размер полученных данных, поэтому ставим метку конца строки
				 В буфере запроса.*/
				buf[result] = '\0';

				/* Данные успешно получены
				формируем тело ответа (HTML)*/
				response_body << "<title>Test C++ HTTP Server</title>\n"
					<< "<h1>Test page</h1>\n"
					<< "<p>This is body of the test page...</p>\n"
					<< "<h2>Request headers</h2>\n"
					<< "<pre>" << buf << "</pre>\n"
					<< "<em><small>Test C++ Http Server</small></em>\n";

				// Формируем весь ответ вместе с заголовками
				response << "HTTP/1.1 200 OK\r\n"
					<< "Version: HTTP/1.1\r\n"
					<< "Content-Type: text/html; charset=utf-8\r\n"
					<< "Content-Length: " << response_body.str().length()
					<< "\r\n\r\n"
					<< response_body.str();

				// Отправляем ответ клиенту с помощью функции send
				result = send(client_socket, response.str().c_str(),
					response.str().length(), 0);

				if (result == SOCKET_ERROR) {
					// произошла ошибка при отправле данных
					cerr << "send failed: " << WSAGetLastError() << "\n";
				}
				// Закрываем соединение к клиентом
				closesocket(client_socket);
			}

			// Убираем за собой
			closesocket(listen_socket);
			freeaddrinfo(addr);
			WSACleanup();
			return 0;
}

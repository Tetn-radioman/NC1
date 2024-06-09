#define WINVER 0x0500//это, чтобы проверить во время компиляции, будет ли приложение работать, например, в Windows 2000 (0x0500) или в Windows XP (0x0501).
#define _CRT_SECURE_NO_WARNINGS/*
1. "define" - это ключевое слово, используемое для определения макросов препроцессора.

2. "crt" - это сокращение от "C runtime" (рантайм-библиотека C). Оно указывает, 
что директива относится к настройкам компилятора, связанным с использованием стандартной библиотеки C.

3. "secure" - это параметр, который указывает, что должны быть включены безопасные настройки компилятора.
Зачастую включение этого параметра подразумевает использование функций, 
которые обеспечивают защиту от уязвимостей типичных для C/C++ (например, переполнение буфера).

4. "no warnings" - это параметр, который указывает, что компилятор должен игнорировать предупреждения,
которые обычно выводятся при компиляции кода. Это может быть полезно, если вы уверены, что код написан корректно и не нуждается в предупреждениях.
*/
#include <iostream>//заголовочный файл с классами, функциями и переменными для организации ввода-вывода
#include <fstream>//заголовочный файл, включает в себя набор классов, методов и функций, которые предоставляют интерфейс для чтения и записи данных из и в файл.
#include <string>//класс с методами и переменными для организации работы со строками
#include <windows.h>//это Windows-специфичный заголовочный файл языка программирования С, в котором объявляются функции,
                    //предоставляющие интерфейс доступа к Windows API


using namespace std;

int x = 0, y = 0;
string adress_sel = "C:\\Games\\NCV2\\Select_container.txt";

//https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes <---ссылка на коды всех клавиш
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


int get_commands()
{
	INPUT ip;
	fstream out;
	out.open(adress_sel);
	if (out.is_open())
	{
		ifstream in(adress_sel);
		string str;
		getline(in, str);

		for (int i = 0; i != 100; i++) {
			
			getline(in, str);
			//установить позицию курсора по X
			if (str == "moveMX")
			{
				getline(in, str);
				SetCursorPos(x = atoi(str.c_str()), y);//string преобразуется в int
			}
			//установить позицию курсора по Y
			if (str == "moveMY")
			{
				getline(in, str);
				SetCursorPos(x, y = atoi(str.c_str()));//string преобразуется в int
			}
			//клик левой кнопки мыши
			if (str == "LclickM")
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
			}
			//клик правой кнопки мыши
			if (str == "RclickM")
			{
				mouse_event(MOUSEEVENTF_RIGHTDOWN, x, y, 0, 0);
				mouse_event(MOUSEEVENTF_RIGHTUP, x, y, 0, 0);

			}
			//нажатие стрелок
			if (str == "left")
			{
				ip.type = INPUT_KEYBOARD;
				ip.ki.wScan = 0; // hardware scan code for key
				ip.ki.time = 0;
				ip.ki.dwExtraInfo = 0;

				ip.ki.wVk = 37; // virtual-key 
				ip.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &ip, sizeof(INPUT));
			}
			if (str == "right")
			{
				ip.type = INPUT_KEYBOARD;
				ip.ki.wScan = 0; // hardware scan code for key
				ip.ki.time = 0;
				ip.ki.dwExtraInfo = 0;

				ip.ki.wVk = 39; // virtual-key 
				ip.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &ip, sizeof(INPUT));
			}
			if (str == "down")
			{
				ip.type = INPUT_KEYBOARD;
				ip.ki.wScan = 0; // hardware scan code for key
				ip.ki.time = 0;
				ip.ki.dwExtraInfo = 0;

				ip.ki.wVk = 40; // virtual-key 
				ip.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &ip, sizeof(INPUT));
			}
			if (str == "up")
			{
				ip.type = INPUT_KEYBOARD;
				ip.ki.wScan = 0; // hardware scan code for key
				ip.ki.time = 0;
				ip.ki.dwExtraInfo = 0;

				// Press the "A" key
				ip.ki.wVk = 38; // virtual-key code
				ip.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &ip, sizeof(INPUT));
			}
			//нажатие клавиши Win
			if (str == "Win")
			{
				keybd_event(91, 0, KEYEVENTF_EXTENDEDKEY, 0);
				Sleep(1);
				keybd_event(91, 0, KEYEVENTF_KEYUP, 0);
				/* Windows:91 */
			}
			//нажатие клавиши ESC
			if (str == "ESC")
			{
				keybd_event(27, 0, KEYEVENTF_EXTENDEDKEY, 0);
				Sleep(1);
				keybd_event(27, 0, KEYEVENTF_KEYUP, 0);
				/* ESC:27 */
			}
			//открытие N-ой ссылки
			if (str == "openWeb")
			{
				getline(in, str);

				//std::string в lpcwstr 
				wstring stemp = wstring(str.begin(), str.end());
				LPCWSTR sw = stemp.c_str();

				ShellExecute(0, 0, sw, 0, 0, SW_SHOW);
			}
			//остановка на N милесекунд
			if (str == "SLEEP")//остановка на N милесекунд
			{
				getline(in, str);
				Sleep(atoi(str.c_str()));
			}
			//завершение выполнения команд
			if (str == "end")
			{
				out.close();
				break;
			}
			// открытие контейнеров
			if (str == "BLITZ")
			{
				out.close();
				int select;//переменная отвечающая за ввод пользователя
				bool selB = true;//флаг продолжения работы алгоритма
				//1-обычный(7мин+7мин+7мин рестат 24ч)
				//2-редкий(3дня)
				//3-эпик(7дней)
				while (selB)
				{
					system("cls");//отчистка консоли
					cout << "1 - обычный (7мин*3шт рестат 24ч)\n2 - редкий (3дня)\n3 - эпик (7дней)\nYour choise:";
					if (!(cin >> select) || cin.get() != '\n')
					{
						cout << "Incorrect input!" << endl;
						Sleep(3000);
						cin.clear(); // сбрасывает все биты потока, тоесть поток становится "хорошим"
						cin.sync();//Удалим данные из буффера
					}
					else
					{
						switch (select)//изменение адреса 
						{
						case 1:
							adress_sel = "C:\\Games\\NCV2\\small.txt";
							selB = false;
							break;
						case 2:
							adress_sel = "C:\\Games\\NCV2\\medium.txt";
							selB = false;
							break;
						case 3:
							adress_sel = "C:\\Games\\NCV2\\large.txt";
							selB = false;
							break;
						default:
							cout << "Unsupperted choise! Enter a number from 1 to 3!" << endl;
							Sleep(3000);
							break;
						}
					}
				}

				get_commands();//рекурсия

			}
		}
	}
	else cout << "Error!\nFailed file opening!\n";//провал открыттия файла

	return 0;
}


int main()
{
	setlocale(LC_ALL, "ru");
	//ShowWindow(GetConsoleWindow(), SW_HIDE); //скрыть консоль
	SetCursorPos(x, y);
	get_commands();//выполнение команд


	return 0;
}
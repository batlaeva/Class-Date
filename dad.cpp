// dad.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <math.h>
#include <string>
#include <ctime>
using namespace std;

class Bad_Area {};
class Bad{};

class Data {
public:
	int day, month, year;
	Data() {
		day = 1;
		month = 1;
		year = 2017;
	}
	Data(int d, int m, int y) {
		day = d;
		month = m;
		year = y;
	}

	//ОПЕРАТОРЫ ВВОДА/ВЫВОДА
	friend istream& operator >>(istream& in, Data& d) {
		cout << "Введите дату (через . или /, в формате дд.мм.гггг, год не меньше 1917) :"<<endl;
		string s, s2;
		int k = 0, y = 0;//k-для парсинга, y-для определения день или месяц
		in >> s;
		for (int i = 0; i < s.size(); i++)
			if (s[i] == '.' || s[i] == '/') {
				s2 = s.substr(k, i - k);//копирование подстроки
				k = i + 1;
				if (s2.size() == 1) s2 = "0" + s2;//если введено <10
				if (isdigit(s2[0]) && isdigit(s2[1]))//цифра ли это?
					(y == 0) ? d.day = stoi(s2) : d.month = stoi(s2);
				else throw Bad_Area();//обработка исключений
				y++;
			}
			else if (!isdigit(s[i])) throw Bad_Area();//если введена не цифра или . или /
		s2 = s.substr(k, s.size() - k);
		if (isdigit(s2[0]) && isdigit(s2[1]) && isdigit(s2[2]) && isdigit(s2[3])) d.year = stoi(s2);
		else throw Bad_Area();
		//проверка на ввод правильных данных типа день не больше 31 и тд.
		if (d.year<1917 || d.month<1 || d.month>12 || d.day<1 || d.day>dofm(d.month, d.year)) throw Bad();
		return in;
	}

	friend ostream& operator <<(ostream& out, const Data& d) {
		//cout << "В каком формате вывести?" << endl << "1) 1.01.2001" << endl << "2) 1 января 2001" << endl << "3) 01.01.01" << endl;
		int i;
		//cin >> i;
		i = 2;
		int y;
		string m[13] = { "","января","февраля","марта","апреля","мая","июня","июля","августа","сентябр","октября","ноября","декабря" };
		switch (i)
		{
		case 1:
			if (d.month<10) out << d.day << ".0" << d.month << "." << d.year << endl;
			else out << d.day <<"."<< d.month << "." << d.year << endl;
			break;
		case 2:
			out << d.day << " " << m[d.month] << " " << d.year << endl;
			break;
		case 3:
			y = d.year % 100;
			if (d.day<10) out << "0" << d.day;
			else out << d.day;
			if (d.month<10) out << ".0" << d.month;
			else out <<"." << d.month;
			if (y<10) out << "0" << d.year << endl;
			else out << d.year << endl;
			break;
		default:
			break;
		}
		return out;
	}

	//вычислять расстояние между датами в днях
	int rast(Data& a, Data& b) {
		return abs(ves(a) - ves(b));
	}

	//день недели
	string day_of_week(Data& a) {
		string dofw[7] = { "Воскресенье","Понедельник","Вторник","Среда","Четверг","Пятница","Суббота" };
		Data b(31, 12, 1916);
		int i;
		i = rast(a, b) % 7;
		if (i == 7) i = 0;
		return dofw[i];
	}
	//hay birhday
	void hb(Data& a) {
		time_t now = time(0);
		tm *ltm = localtime(&now);
		Data b(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
		Data c(a.day, a.month, 1900 + ltm->tm_year);
		cout <<"Сегодня : "<< day_of_week(b)<<", "<< b<<endl;
		//cout << "Ваш ДР был в  : " << day_of_week(a) << endl;
		if (a.day == b.day && a.month == b.month) cout << "Happy Birthday!!!";
		if (a.month<b.month || (a.month == b.month && a.day<b.day)) cout << "Ваш день рождения прошел : " << rast(c, b);
		if (a.month>b.month || (a.month == b.month && a.day>b.day)) cout << "До вашего дня рождения осталось дней : " << rast(c, b);
	}

private:
	//проверка на високосность
	bool vis(int y) {
		return ((y % 400 == 0) || (y % 4 == 0 && y % 100 != 0)) ? true : false;
	}
	//вычислить количество дней  в дате
	int ves(Data& a) {
		int d = 0;//сумма дней
				  //вычисление в годах
		for (int i = 1916; i<a.year; i++)
			(vis(i)) ? d += 366 : d += 365;
		//вычисление в месяцах
		for (int i = 1; i<a.month; i++)
			d+=dofm(i, a.year);
		d += a.day;
		return d;
	}

	static int dofm(int i,int y) {
		if (i != 2 && ((i<8 && i % 2 == 1) || (i>7 && i % 2 == 0))) return 31;
		else if (i != 2) return 30;
		if (i == 2 && (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0))) return 29;//февраль
		else if (i == 2) return 28;
	}
};

void main()
{
	setlocale(LC_ALL, "rus");
	Data b;
	try
	{
		cin >> b;
		(new Data())->hb(b);
	}
	catch (Bad_Area)
	{
		cout << "Дата введена неправильно";
	}
	catch (Bad)
	{
		cout << "Дата несовместима, <0, >31 ...";
	}
	_getch();
}


#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include<map>

//#include <openssl/sha.h>

using namespace std;

/*1. ¬ести список или структуру данных дл€ хранени€ информации о каждом подключенном клиенте. Ёта информаци€ может включать данные о клиенте, такие как им€ пользовател€ и комната, в которой он находитс€ в данный момент.
2.  огда клиент подключаетс€ к серверу, присвойте ему уникальный идентификатор (например, ID клиента) и добавьте запись о клиенте в список или структуру данных, созданную на шаге 1.
3. –азрешить клиентам указывать комнату, к которой они хот€т присоединитьс€. Ёто можно сделать с помощью команды или взаимодействи€ с пользовательским интерфейсом.
4.  огда клиент отправл€ет сообщение, проверьте, к какой комнате он принадлежит. “ранслируйте сообщение только клиентам, наход€щимс€ в данной комнате.
5. ѕредоставить клиентам возможность переключени€ между комнатами, если они хот€т присоединитьс€ к другой комнате. ќбновить информацию о комнате дл€ соответствующего клиента в списке или структуре данных.
*/

class User
{
private:
	int id;
	string name;
	string hasedPassword;
	map <int, string> room;
	//map <int, string> frend_list;


public:
	User(){}
	User(int id, string name, string password) {
		this->id = id;
		this->name = name;
		this->hasedPassword = password;
	}
		

public:
	int get_id() {
		return id;
	}
	void set_id(int val) {
		id = val;
	}

	string get_name() {
		return name;
	}
	void set_name(string val) {
		name = val;
	}

	string get_pass() {
		return hasedPassword;
	}
	void set_pass(string val) {
		hasedPassword = val;
	}

	map<int, string>::iterator get_room() {
		map<int, string>::iterator it = this->room.begin();
		return it;
	}
	void set_room(int room_num, string room_name)
	{
		this->room.emplace(room_num, room_name);
	}
	

	//int get_room_num() {
	//	return room_num;
	//}
	//void set_room_num(int val) {
	//	room_num = val;
	//}
};


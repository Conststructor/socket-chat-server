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

/*1. ����� ������ ��� ��������� ������ ��� �������� ���������� � ������ ������������ �������. ��� ���������� ����� �������� ������ � �������, ����� ��� ��� ������������ � �������, � ������� �� ��������� � ������ ������.
2. ����� ������ ������������ � �������, ��������� ��� ���������� ������������� (��������, ID �������) � �������� ������ � ������� � ������ ��� ��������� ������, ��������� �� ���� 1.
3. ��������� �������� ��������� �������, � ������� ��� ����� ��������������. ��� ����� ������� � ������� ������� ��� �������������� � ���������������� �����������.
4. ����� ������ ���������� ���������, ���������, � ����� ������� �� �����������. ������������ ��������� ������ ��������, ����������� � ������ �������.
5. ������������ �������� ����������� ������������ ����� ���������, ���� ��� ����� �������������� � ������ �������. �������� ���������� � ������� ��� ���������������� ������� � ������ ��� ��������� ������.
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


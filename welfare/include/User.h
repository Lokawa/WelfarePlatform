
#pragma once

#include <iostream>
#include <string>

class User
{
public:
	int id;
	User(std::string username, std::string password)
		: username(username), password(password) {
	}
	User(){}
	User(int id) {
		this->id = id;
	}
	std::string username;
	std::string password;
};

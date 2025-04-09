# pragma once
#include <string>
#include <iostream>
#include "UserDAO.h"
#include "crow.h"
class UserService 
{
private:
	UserDAO DAO{ "../data/user.db" };
	bool Login(std::string username,std::string password)
	{
		if (DAO.Login_check(username, password))
		{
			std::cout << "Login successful!" << std::endl;
			return true;
		}
		else
		{
			std::cout << "Login failed!" << std::endl;
			return false;
		}
	}
	void Register(std::string username, std::string password)
	{
		DAO.UserAdd(username, password);
		std::cout << "Register successful!" << std::endl;
	}
	void Delete(std::string username)
	{
		DAO.UserDelete(username);
		std::cout << "Delete successful!" << std::endl;
	}
	bool PasswordChange(std::string username, std::string old_password, std::string new_password)
	{
		if (DAO.Login_check(username, old_password))
		{
			DAO.UserModifyPassword(username, new_password);
			std::cout << "Password change successful!" << std::endl;
			return true;
		}
		else
		{
			std::cout << "Password change failed!" << std::endl;
			return false;
		}
	}
public:
	UserService() {}

	~UserService() = default;
	void route(crow::SimpleApp& app)
	{
		// µÇÂ¼
		CROW_ROUTE(app, "/user/login").methods("POST"_method)([this](const crow::request& req) {
			auto body = crow::json::load(req.body);
			if (!body) return crow::response(400, "Invalid JSON");

			std::string username = body["username"].s();
			std::string password = body["password"].s();

			bool success = this->Login(username, password);
			return success ? crow::response(200, "Login successful")
				: crow::response(401, "Login failed");
			});

		// ×¢²á
		CROW_ROUTE(app, "/user/register").methods("POST"_method)([this](const crow::request& req) {
			auto body = crow::json::load(req.body);
			if (!body) return crow::response(400, "Invalid JSON");

			std::string username = body["username"].s();
			std::string password = body["password"].s();

			this->Register(username, password);
			return crow::response(200, "Register successful");
			});

		// É¾³ý
		CROW_ROUTE(app, "/user/delete").methods("POST"_method)([this](const crow::request& req) {
			auto body = crow::json::load(req.body);
			if (!body) return crow::response(400, "Invalid JSON");

			std::string username = body["username"].s();
			this->Delete(username);
			return crow::response(200, "Delete successful");
			});

		// ÐÞ¸ÄÃÜÂë
		CROW_ROUTE(app, "/user/change_password").methods("POST"_method)([this](const crow::request& req) {
			auto body = crow::json::load(req.body);
			if (!body) return crow::response(400, "Invalid JSON");

			std::string username = body["username"].s();
			std::string old_pw = body["old_password"].s();
			std::string new_pw = body["new_password"].s();

			if (this->PasswordChange(username, old_pw, new_pw)) {
				return crow::response(200, "Password change successful");
			}
			else {
				return crow::response(403, "Old password incorrect");
			}
			});
	}
};
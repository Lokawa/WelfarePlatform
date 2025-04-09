#pragma once 
#include <string>
#include <iostream>
#include <sqlite3.h>
#include "User.h"
class UserDAO
{
private:
	std::string sql_path;
	static int callback(void* data, int argc, char** argv, char** azColName)
	{
		// 在回调中处理查询结果
		User* user = (User*)data;
		if (argc > 0) {
			user->id = std::stoi(argv[0]);  // 假设 id 在第一列
			user->username = argv[1];       // 假设 username 在第二列
			user->password = argv[2];
		}
		return 0;  // 必须返回 0
	}
public:
	UserDAO(std::string sql_path)
	{
		this->sql_path = sql_path;
	}
	~UserDAO(){}
	void UserAdd(std::string username,std::string password)
	{
		sqlite3* db;
		
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return;
		}
		std::string sql = "INSERT INTO user (username, password) VALUES ('" + username + "', '" + password + "');";
		std::cout << sql<< std::endl;
		char* errMsg = 0;
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
		}
		sqlite3_close(db);
	}
	void UserDelete(std::string username)
	{
		sqlite3* db;
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return;
		}
		std::string sql = "DELETE FROM user WHERE username = '" + username + "';";
		char* errMsg = 0;
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
		}
		sqlite3_close(db);
	}
	void UserModifyPassword(std::string username, std::string password)
	{
		sqlite3* db;
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return;
		}
		std::string sql = "UPDATE user SET password = '" + password + "' WHERE username = '" + username + "';";
		char* errMsg = 0;
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
		}
		sqlite3_close(db);
	}
	User Get_user(int id)
	{
		sqlite3* db;
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return User(-1);
		}
		std::string sql = "SELECT * FROM user WHERE id = " + std::to_string(id) + ";";
		char* errMsg = 0;
		User user;
		rc = sqlite3_exec(db, sql.c_str(), callback,&user , &errMsg);
		
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			return User(-1);
		}
		sqlite3_close(db);
		return user;
	}
	bool Login_check(std::string username, std::string password)
	{
		sqlite3* db;
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return false;
		}
		std::string sql = "SELECT * FROM user WHERE username = '" + username + "';";
		char* errMsg = 0;
		User user;
		rc = sqlite3_exec(db, sql.c_str(), callback, &user, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			return false;
		}
		if (user.password == password)
		{
			std::cout << "Login successful!" << std::endl;
		}
		else
		{
			std::cout << "Login failed!" << std::endl;
			sqlite3_free(errMsg);
			return false;
		}
		sqlite3_close(db);
		return true;
	}
		
};
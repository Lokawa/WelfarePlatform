#pragma once
#include <vector>
#include "Welfare.h"
#include <sqlite3.h>
using namespace std;
class WelfareDAO
{
private:
	std::string sql_path;
	static int callback(void* data, int argc, char** argv, char** azColName)
	{
		// 在回调中处理查询结果
		Welfare* welfare = (Welfare*)data;
		if (argc > 0) {
			welfare->id = std::stoi(argv[0]);  // 假设 id 在第一列
			welfare->name = argv[1];       // 假设 username 在第二列
			welfare->description = argv[2];
			welfare->type = std::stoi(argv[3]);// 假设 type 在第四列
		}
		return 0;  // 必须返回 0
	}
	static int callback_list(void* data, int argc, char** argv, char** azColName)
	{
		auto* list = static_cast<std::vector<Welfare>*>(data);
		Welfare item;
		if (argc > 0) {
			item.id = std::stoi(argv[0]);  // 假设 id 在第一列
			item.name = argv[1];       // 假设 username 在第二列
			item.description = argv[2];
			item.type = std::stoi(argv[3]); // 假设 type 在第四列
		}
		list->push_back(item);
		return 0;  // 必须返回 0
	}
public:
	WelfareDAO(std::string sql_path)
	{
		this->sql_path = sql_path;
	}
	~WelfareDAO() {}
	void WelfareAdd(std::string name, std::string description, int type,std::vector<std::string> image_urls)
	{
		sqlite3* db;

		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return;
		}
		std::string sql = "INSERT INTO welfare (name, description, type) VALUES ('" + name + "', '" + description + "', '" + std::to_string(type) + "');";
		std::cout << sql << std::endl;
		char* errMsg = 0;
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
			return;
		}

		int id = sqlite3_last_insert_rowid(db);
		for (const auto& image_url : image_urls)
		{
			std::string sql = "INSERT INTO welfare_image (welfare_id, image_url) VALUES (?, ?);";
			sqlite3_stmt* stmt;
			rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
			if (rc != SQLITE_OK)
			{
				std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
				sqlite3_close(db);
				return;
			}
			sqlite3_bind_int(stmt, 1, id);
			sqlite3_bind_text(stmt, 2, image_url.c_str(), -1, SQLITE_STATIC);
			rc = sqlite3_step(stmt);
			if (rc != SQLITE_DONE)
			{
				std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
			}
			sqlite3_finalize(stmt);
		}
		sqlite3_close(db);
	}
	void WelfareDelete(std::string name)
	{
		sqlite3* db;
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return;
		}
		std::string sql = "DELETE FROM welfare WHERE name = '" + name + "';";
		char* errMsg = 0;
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
		}
		sqlite3_close(db);
	}
	Welfare Get_welfare(int id)
	{
		sqlite3* db;
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return Welfare(-1);
		}
		std::string sql = "SELECT * FROM welfare WHERE id = " + std::to_string(id) + ";";
		char* errMsg = 0;
		Welfare welfare;
		rc = sqlite3_exec(db, sql.c_str(), callback, &welfare, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			return Welfare(-1);
		}
		sqlite3_close(db);
		return welfare;
	}
	vector<std::string> Get_welfare_image(int id)
	{
		sqlite3* db;
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return vector<std::string>();
		}
		std::string sql = "SELECT image_url FROM welfare_image WHERE welfare_id = " + std::to_string(id) + ";";
		char* errMsg = 0;
		vector<std::string> image_urls;
		rc = sqlite3_exec(db, sql.c_str(), callback_list, &image_urls, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			return vector<std::string>();
		}
		sqlite3_close(db);
		return image_urls;
	}
	vector<Welfare> SearchWelfare(int type)
	{
		sqlite3* db;
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return vector<Welfare>();
		}
		std::string sql = "SELECT * FROM welfare WHERE type = " + std::to_string(type) + ";";
		char* errMsg = 0;
		vector<Welfare> welfare_list;
		rc = sqlite3_exec(db, sql.c_str(), callback_list, &welfare_list, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			return vector<Welfare>();
		}
		sqlite3_close(db);
		return welfare_list;
	}
	vector<Welfare> SearchWelfare()
	{
		sqlite3* db;
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return vector<Welfare>();
		}
		std::string sql = "SELECT * FROM welfare;";
		char* errMsg = 0;
		vector<Welfare> welfare_list;
		rc = sqlite3_exec(db, sql.c_str(), callback_list, &welfare_list, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			return vector<Welfare>();
		}
		sqlite3_close(db);
		return welfare_list;
	}
	void WelfareModify(std::string name, string description, int type, const std::vector<std::string>& image_urls)
	{
		sqlite3* db;
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return;
		}
		std::string sql = "UPDATE welfare SET description = '" + description + "', type = '" + std::to_string(type) + "' WHERE name = '" + name + "';";
		char* errMsg = 0;
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
			return;
		}
		
		//get ID
		int welfare_id;
		sql = "SELECT id FROM welfare WHERE name = '" + name + "';";
		sqlite3_stmt* stmt;
		rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
		if (rc != SQLITE_OK) {
			std::cerr << "SQL error (prepare select welfare_id): " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
			return;
		}

		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW) {
			welfare_id = sqlite3_column_int(stmt, 0);  // 获取 welfare_id
		}
		else {
			std::cerr << "No welfare found with name: " << name << std::endl;
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return;
		}
		sqlite3_finalize(stmt);

		// 删除旧的图片记录
		sql = "DELETE FROM welfare_image WHERE welfare_id = " + std::to_string(welfare_id) + ";";
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
			sqlite3_close(db);
			return;
		}

		// 重新插入图片
		for (const auto& image_url : image_urls)
		{
			std::string sql = "INSERT INTO welfare_image (welfare_id, image_url) VALUES (?, ?);";
			sqlite3_stmt* stmt;
			rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
			if (rc != SQLITE_OK)
			{
				std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
				sqlite3_close(db);
				return;
			}
			sqlite3_bind_int(stmt, 1, welfare_id);
			sqlite3_bind_text(stmt, 2, image_url.c_str(), -1, SQLITE_STATIC);
			rc = sqlite3_step(stmt);
			if (rc != SQLITE_DONE)
			{
				std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
			}
			sqlite3_finalize(stmt);
		}


		sqlite3_close(db);
	}
	void WelfareVisit(int id)
	{
		sqlite3* db;
		int rc = sqlite3_open(sql_path.c_str(), &db);
		if (rc)
		{
			std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
			return;
		}
		std::string sql = "UPDATE welfare SET frequency = frequency + 1 WHERE id = " + std::to_string(id) + ";";
		char* errMsg = 0;
		rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			std::cerr << "SQL error: " << errMsg << std::endl;
			sqlite3_free(errMsg);
		}
		sqlite3_close(db);
	}

};
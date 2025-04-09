#pragma once
#include <string>
#include <iostream>
#include "WelfareDAO.h"
#include "crow.h"
class WelfareService 
{
private:
	WelfareDAO DAO{ "../data/welfare.db" };
	void WelfareAdd(std::string name, std::string description, int type)
	{
		DAO.WelfareAdd(name, description, type);
		std::cout << "Welfare add successful!" << std::endl;
	}
	void WelfareDelete(std::string name)
	{
		DAO.WelfareDelete(name);
		std::cout << "Welfare delete successful!" << std::endl;
	}
	void WelfareModify(std::string name, std::string description, int type)
	{
		DAO.WelfareModify(name, description, type);
		std::cout << "Welfare modify successful!" << std::endl;
	}
public:
	WelfareService() {}
	~WelfareService() = default;
	void route(crow::SimpleApp& app)
	{
		// 添加福利
		CROW_ROUTE(app, "/welfare/add").methods("POST"_method)([this](const crow::request& req) {
			auto body = crow::json::load(req.body);
			if (!body) return crow::response(400, "Invalid JSON");
			std::string name = body["name"].s();
			std::string description = body["description"].s();
			int type = body["type"].i();
			WelfareAdd(name, description, type);
			return crow::response(200, "Welfare added successfully");
			});
		// 删除福利
		CROW_ROUTE(app, "/welfare/delete").methods("POST"_method)([this](const crow::request& req) {
			auto body = crow::json::load(req.body);
			if (!body) return crow::response(400, "Invalid JSON");
			std::string name = body["name"].s();
			WelfareDelete(name);
			return crow::response(200, "Welfare deleted successfully");
			});
		// 修改福利
		CROW_ROUTE(app, "/welfare/modify").methods("POST"_method)([this](const crow::request& req) {
			auto body = crow::json::load(req.body);
			if (!body) return crow::response(400, "Invalid JSON");
			std::string name = body["name"].s();
			std::string description = body["description"].s();
			int type = body["type"].i();
			WelfareModify(name, description, type);
			return crow::response(200, "Welfare modified successfully");
			});
	}
};
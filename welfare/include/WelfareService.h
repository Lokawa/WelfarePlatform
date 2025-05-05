#pragma once
#include <string>
#include <iostream>
#include "WelfareDAO.h"
#include "crow.h"
class WelfareService 
{
private:
	WelfareDAO DAO{ "../data/welfare.db" };
	void WelfareAdd(std::string name, std::string description, int type,std::vector<std::string> image_urls)
	{
		DAO.WelfareAdd(name, description, type, image_urls);
		std::cout << "Welfare add successful!" << std::endl;
	}
	void WelfareDelete(std::string name)
	{
		DAO.WelfareDelete(name);
		std::cout << "Welfare delete successful!" << std::endl;
	}
	void WelfareModify(std::string name, std::string description, int type, std::vector<std::string> image_urls)
	{
		DAO.WelfareModify(name, description, type, image_urls);
		std::cout << "Welfare modify successful!" << std::endl;
	}
	std::pair<Welfare, std::vector<std::string>> WelfareDetail(int id)
	{
		// 获取 Welfare 记录
		DAO.WelfareVisit(id);
		Welfare welfare = DAO.Get_welfare(id);
		std::cout << "Welfare detail show successful!" << std::endl;

		// 获取相关的图片 URL
		std::vector<std::string> image_urls = DAO.Get_welfare_image(id);

		// 返回 Welfare 和图片 URL 列表
		return std::make_pair(welfare, image_urls);
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
			std::vector<std::string> image_urls;
			if (body.has("image_urls")) {
				for (const auto& url : body["image_urls"]) {
					image_urls.push_back(url.s());
				}
			}
			WelfareAdd(name, description, type, image_urls);
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
			std::vector<std::string> image_urls;
			if (body.has("image_urls")) {
				for (const auto& url : body["image_urls"]) {
					image_urls.push_back(url.s());
				}
			}
			WelfareModify(name, description, type, image_urls);
			return crow::response(200, "Welfare modified successfully");
			});
		// 福利详情
		CROW_ROUTE(app, "/welfare/detail/<int>").methods("GET"_method)([this](const crow::request& req, int id) {
			auto welfare_data= WelfareDetail(id);
			Welfare welfare = welfare_data.first;
			crow::json::wvalue welfare_json;
			welfare_json["id"] = welfare.id;
			welfare_json["name"] = welfare.name;
			welfare_json["description"] = welfare.description;
			welfare_json["type"] = welfare.type;
			welfare_json["frequency"] = welfare.frequency;
			crow::json::wvalue image_urls_json = crow::json::wvalue();
			int index = 0;
			for (const auto& url : welfare_data.second) {
				image_urls_json[index++] = url;  // 使用索引添加图片 URL
			}
			welfare_json["image_urls"] =  std::move(image_urls_json);
			return crow::response(200, welfare_json.dump());
			});
	}
};
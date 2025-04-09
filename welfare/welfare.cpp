// welfare.cpp: 定义应用程序的入口点。
//


#include "include/WelfareService.h"
#include "include/UserService.h"
#include <iostream>
using namespace std;

int main()
{
	//UserDAO USERDAO{ "../data/user.db" };
	//USERDAO.UserAdd("alice","123456");
	//if (USERDAO.Login_check("alice", "12345")) cout << "Correct" << endl; else cout << "Wrong" << endl;
	crow::SimpleApp app;

	UserService userService;
	userService.route(app);

	app.port(18080).multithreaded().run();
	return 0;
}

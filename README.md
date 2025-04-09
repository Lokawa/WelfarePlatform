# WelfarePlatform

## 环境

项目为VS2022 Cmake项目，sql数据库位置为/out/build/x64-debug/data。使用crow(asio依赖)和sqlite3库实现数据更新和网络通信。

## 接口

- UserSevice和WelfareSevice均有route函数做前端指令解析交互。目前main函数仅有UserService实例（测试用），后续可以修改。

- 目前使用curl通信，本地端口18080。运行out/build/x64-debug文件夹内部的exe后再开一个终端输入curl命令可以实现数据更新。
    - 例：注册用户testuser   
    curl -X POST http://localhost:18080/user/register -H "Content-Type: application/json" -d "{\"username\":\"testuser\",\"password\":\"123456\"}"

- 具体需要传入的数据为：
    - user:
        - login: username, password
        - register: username, password
        - delete: username
        - change_password: username, old_password, new_password（有认证）
    - welfare:
        - add: name, description, type
        - delete: name
        - modify: name, description, type
## 数据库格式
user.db


    id  username  password
    --  --------  --------
    1   testuser  123456

welfare.db

    id  name         description  type
    --  -----------  -----------  ----
    2   testwelfare  test         1


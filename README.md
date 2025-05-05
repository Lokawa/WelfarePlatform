# WelfarePlatform

## 环境

项目为VS2022 Cmake项目，sql数据库位置为/out/build/x64-debug/data。使用crow(asio依赖)和sqlite3库实现数据更新和网络通信。

## 接口

- UserSevice和WelfareSevice均有route函数做前端指令解析交互。目前main函数有UserService和WelfareService实例（测试可用）。

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
        - add: name, description, type，image_urls
        - delete: name
        - modify: name, description, type，image_urls
        - detail：id

- 2025.5 更新支持点击频率和图片
    - image_urls: 用户上传的图片url数组。url建议使用图床存储
    - frequency： 每次调用detail函数自动加一
## 数据库格式
user.db


    id  username  password
    --  --------  --------
    1   testuser  123456

welfare.db
- welfare表
      
        id  name      description  frequency  type
        --  --------  -----------  ---------  ----
        1   testname  test         3          0

- welfare_image表
  
        id  welfare_id  image_url
        --  ----------  ---------------------------
        1   1           http://example.com/img1.png
        2   1           http://example.com/img2.png
      



# CMakeList.txt: welfare 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 将源代码添加到此项目的可执行文件。
include_directories(crow/include)
include_directories(asio/asio/include)
add_subdirectory(sqlite3)
add_executable (welfare "welfare.cpp"  "include/WelfareService.h")
target_link_libraries(welfare sqlite3)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET welfare PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

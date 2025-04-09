# 设置源文件
add_library(sqlite3 STATIC sqlite3.c)

# 指定包含目录
target_include_directories(sqlite3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

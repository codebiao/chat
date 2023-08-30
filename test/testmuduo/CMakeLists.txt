# 配置头文件搜索路径
# include_directories()
# 配置库文件搜索路径
# link_directories()

# 设置需要编译的源文件列表
set(SRC_LIST ./muduo_server.cpp)

# 将.指定路径下的所有源文件名字放入变量名SRC_LIST中
#aux_source_directory(. SRC_LIST)

# 2、链接源文件
add_executable(server ${SRC_LIST}) 

# 3、链接库
target_link_libraries(server muduo_net muduo_base pthread)

# 设置输出文件路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)   # 设置生成的可执行文件保存路径


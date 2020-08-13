cmake_minimum_required (VERSION 3.12.1)

project(tt_server)


aux_source_directory(. DIR_SRCS)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -pthread ")



# 添加子目录
add_subdirectory(net)
add_subdirectory(http)
add_subdirectory(base)
#add_subdirectory(test)

# 指定生成目标
add_executable(tt_server ${DIR_SRCS})

# 添加链接库

target_link_libraries(tt_server NetFunctions)
#target_link_libraries(tt_server TestFunctions)
target_link_libraries(tt_server BaseFunctions)
target_link_libraries(tt_server HttpFunctions)





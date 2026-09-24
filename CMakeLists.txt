cmake_minimum_required(VERSION 3.20)

project(GrowCode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(growcode
    backend/src/server/main.cpp
    backend/src/database/Database.cpp
)

target_include_directories(growcode PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/include
    /opt/homebrew/opt/mysql-connector-c++/include
)

target_link_libraries(growcode PRIVATE
    /opt/homebrew/opt/mysql-connector-c++/lib/libmysqlcppconnx.dylib
)
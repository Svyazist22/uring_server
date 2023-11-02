cmake_minimum_required(VERSION 3.0.0)
project(tcp_server VERSION 0.1.0 LANGUAGES C CXX)

add_executable(tcp_server main.cpp process_control.cpp client.cpp server.cpp file.cpp)
target_link_libraries(tcp_server uring)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
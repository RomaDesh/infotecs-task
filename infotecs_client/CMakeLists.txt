cmake_minimum_required (VERSION 3.8)

project ("infotecs_client")

add_executable (infotecs_client "infotecs_client/client_main.cpp" "infotecs_client/user_input.cpp" "infotecs_client/user_input.h" "infotecs_client/Buffer.cpp" "infotecs_client/Buffer.h" "infotecs_client/ServerConnection.cpp" "infotecs_client/ServerConnection.h")

target_link_libraries(infotecs_client pthread)
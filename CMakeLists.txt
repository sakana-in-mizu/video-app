cmake_minimum_required(VERSION 3.20)

project(video-app CXX C)

set(CMAKE_CXX_STANDARD 20)

if (NOT APPLE)
    message(WARNING "This project is only tested on macOS. Building on other platforms is not guaranteed to work.")
endif()

file(GLOB_RECURSE video_app_src src/*.cpp src/*.c)
file(GLOB_RECURSE video_app_inc src/*.h src/*.hpp)

source_group(TREE ${CMAKE_SOURCE_DIR}/src FILES ${video_app_src} ${video_app_inc})

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty)

add_executable(${PROJECT_NAME} ${video_app_src} ${video_app_inc})
target_link_libraries(${PROJECT_NAME}
    ffmpeg
    glad_gl_core_mx_41
    glfw
    spdlog
)

if(NOT DEFINED log_path)
    set(log_path ${CMAKE_BINARY_DIR}/logs)
endif()
configure_file(
    ${CMAKE_SOURCE_DIR}/src/config_template.ini
    ${CMAKE_BINARY_DIR}/config.ini
    @ONLY
)

if(NOT DEFINED config_file_path)
    set(config_file_path  ${CMAKE_BINARY_DIR}/config.ini)
endif()
configure_file(
    ${CMAKE_SOURCE_DIR}/src/config/config_file_path.h.in
    ${CMAKE_BINARY_DIR}/config_file_path.h
    @ONLY
)

target_include_directories(${PROJECT_NAME} PRIVATE src ${CMAKE_BINARY_DIR})
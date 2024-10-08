set(third_party_folder "3rdparty")

if(NOT TARGET glfw)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${third_party_folder})
    set_target_properties(update_mappings PROPERTIES FOLDER ${third_party_folder}/glfw)
endif()

if(NOT TARGET spdlog)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
    set_target_properties(spdlog PROPERTIES FOLDER ${third_party_folder})
endif()

if(NOT TARGET glad_gl_core_mx_41)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glad/cmake)
    glad_add_library(glad_gl_core_mx_41 STATIC REPRODUCIBLE MX API gl:core=4.1 EXTENSIONS NONE)
    set_target_properties(glad_gl_core_mx_41 PROPERTIES FOLDER ${third_party_folder})
endif()

if(NOT TARGET ffmpeg)
    include(cmake/find_ffmpeg.cmake)
endif()
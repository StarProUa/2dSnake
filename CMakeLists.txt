cmake_minimum_required(VERSION 3.5)

project(2dSnake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

set(SOURCES
    source/main.cpp
    source/application.cpp
    source/application.h
   # source/temp.cpp
)

add_subdirectory(external/GLFW)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Snake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

cmake_minimum_required(VERSION 3.0.0)
project(imgui VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED CONFIG)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

include_directories(${Python3_INCLUDE_DIRS})
# include_directories(${Opencv_INCLUDE_DIRS})

include_directories(${OpenCV_INCLUDE_DIRS} PRIVATE ${OpenCV_INCLUDE_DIRS})
#添加链接库
link_libraries(${OpenCV_LIBS} ${OpenCV_LIBS})


add_executable(imgui 
main.cpp 
imgui/imgui.cpp
imgui/imgui_widgets.cpp
imgui/imgui_tables.cpp
imgui/imgui_impl_opengl3.cpp
imgui/imgui_impl_glfw.cpp
imgui/imgui_draw.cpp
controls.cpp 
display.cpp 
getpath.cpp
layout.cpp 
)

target_link_libraries(imgui GL glfw ${CMAKE_DL_LIBS} ${Python3_LIBRARIES} ${Opencv_LIBS})
 
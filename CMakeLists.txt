# CMakeList.txt: проект CMake для CMakeProjectOpenCV; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)
 project(CMakeProjectOpenCV)

set (PROJECT_NAME  "CMakeProjectOpenCV")

set (CMAKE_CXX_COMPILER:FILEPATH=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/bin/Hostx64/x64/cl.exe -D) 
set (CMAKE_LINKER:FILEPATH=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/bin/Hostx64/x64/link.exe -D) 

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}") 
set (OpenCV_DIR f:\\opencv\\build)

set (OpenCV_INCLUDE_DIR f:\\opencv\\build\\include\\opencv2)
set (OpenCV_INCLUDE_DIRm f:\\opencv\\build\\include)
include_directories(${OpenCV_INCLUDE_DIRS} $(OpenCV_INCLUDE_DIRm))

add_executable (CMakeProjectOpenCV "CMakeProjectOpenCV.cpp" "CMakeProjectOpenCV.h" "DispatchCmd.cpp" "DispatchCmd.h")

target_link_libraries(CMakeProjectOpenCV opencv_world451d)


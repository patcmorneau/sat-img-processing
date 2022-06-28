cmake_minimum_required(VERSION 3.13)


project(prosatcimg VERSION 1.0.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(${PROJECT_NAME} main.cpp)

find_package(OpenCV 4.2 REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS} )


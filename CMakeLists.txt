cmake_minimum_required(VERSION 3.13)


project(prosatcimg VERSION 1.0.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(visualize visualize.cpp)
find_package(OpenCV 4.2 REQUIRED)
target_link_libraries(visualize PUBLIC ${OpenCV_LIBS} )

add_executable(process processing_MNWI.cpp)
target_link_libraries(process PUBLIC ${OpenCV_LIBS} )


add_executable(getwater getWater.cpp)
target_link_libraries(getwater PUBLIC ${OpenCV_LIBS} )


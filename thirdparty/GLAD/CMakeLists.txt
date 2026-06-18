# thirdparty/GLAD/CMakeLists.txt

add_library(glad STATIC "${CMAKE_CURRENT_SOURCE_DIR}/glad.c")

# FIX: Changed link_directories to include_directories
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

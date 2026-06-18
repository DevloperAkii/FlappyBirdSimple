add_library(stbi STATIC "stb/stb_image.cpp" "stb/stb_image.h")

target_include_directories(stbi PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
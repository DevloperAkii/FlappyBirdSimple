# FlappyBirdSimple/CMakeLists.txt

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")
file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")

add_executable(FlappyBirdSimple ${MY_SOURCES} ${MY_HEADERS})

# 1. Disable Auto-Includes
target_compile_definitions(FlappyBirdSimple PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(FlappyBirdSimple PRIVATE RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

# 2. Local Source Includes
target_include_directories(FlappyBirdSimple PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")

# 4. Link Libraries
# FIX: Just linking 'glad' now automatically handles its include path!
target_link_libraries(FlappyBirdSimple PRIVATE 
    glfw
    glad
    opengl32
    imgui
    glm
    stbi
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FlappyBirdSimple PROPERTY CXX_STANDARD 23)
endif()

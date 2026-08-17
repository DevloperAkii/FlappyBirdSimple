# FlappyBirdSimple/CMakeLists.txt

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")
file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")

add_executable(FlappyBirdSimple ${MY_SOURCES} ${MY_HEADERS} "${CMAKE_CURRENT_SOURCE_DIR}/resources/Assets/resource.rc")

# Fix: Map MSVC release optimization flags to the custom "Dist" type
if(MSVC)
    set(CMAKE_CXX_FLAGS_DIST "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "Dist compiler flags" FORCE)
    set(CMAKE_C_FLAGS_DIST "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "Dist compiler flags" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_DIST "${CMAKE_EXE_LINKER_FLAGS_RELEASE}" CACHE STRING "Dist linker flags" FORCE)
endif()

# 1. Disable Auto-Includes
target_compile_definitions(FlappyBirdSimple PRIVATE GLFW_INCLUDE_NONE)

# Fix: Checked for "Dist"
if(CMAKE_BUILD_TYPE STREQUAL "Dist")
    target_compile_definitions(FlappyBirdSimple PRIVATE RESOURCE_PATH="resources/")
else()
    target_compile_definitions(FlappyBirdSimple PRIVATE RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
endif()

# Fix: Handled copy logic cleanly for "Dist"
if(CMAKE_BUILD_TYPE STREQUAL "Dist")
    add_custom_command(TARGET FlappyBirdSimple POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "$<TARGET_FILE_DIR:FlappyBirdSimple>/resources"
        COMMENT "Copying resources folder to output directory for Dist build..."
        VERBATIM
    )
endif()
target_link_options(FlappyBirdSimple PRIVATE
    $<$<CONFIG:Dist>:/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup>
)

# 2. Local Source Includes
target_include_directories(FlappyBirdSimple PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")

# 4. Link Libraries
target_link_libraries(FlappyBirdSimple PRIVATE 
    glfw
    glad
    opengl32
    imgui
    glm
    stbi
    miniaudio
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FlappyBirdSimple PROPERTY CXX_STANDARD 23)
endif()

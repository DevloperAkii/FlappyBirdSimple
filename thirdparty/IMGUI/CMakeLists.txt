# 1. Core ImGui files
set(IMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
)

# 2. Backend files
list(APPEND IMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

# 3. Create your STATIC library
add_library(imgui STATIC ${IMGUI_SOURCES})

# FIX: Force PUBLIC definition so both imgui.lib AND FlappyBirdSimple 
# strip the __declspec(dllimport) macro safely.

# 4. Include directories (Make sure the backend folder is visible)
target_include_directories(imgui PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
)

# 5. Link against GLFW and OpenGL libraries
target_link_libraries(imgui PRIVATE glfw)
if(WIN32)
    target_link_libraries(imgui PRIVATE opengl32)
endif()

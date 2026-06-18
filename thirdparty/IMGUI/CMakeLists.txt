# 1. Manually list ONLY the core ImGui files (No wildcards, no examples)
set(IMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp"
)

# 2. Manually add ONLY the backend files your project uses (e.g., GLFW + OpenGL3)
list(APPEND IMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

# 3. Create your library using the clean source list
add_library(imgui STATIC ${IMGUI_SOURCES})

# 4. Expose the directories so your main project can find the headers
target_include_directories(imgui PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends"
)

# 5. Link against your GLFW target
target_link_libraries(imgui PRIVATE glfw)

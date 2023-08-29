function(example_executable target_name)
    SET(executable_name ${PROJECT_NAME}_${target_name})
    add_executable(${executable_name} ${ARGN})
    target_compile_features(${executable_name} PRIVATE cxx_std_20)
    target_link_libraries(${executable_name} PRIVATE OpenGLApp)
endfunction()

example_executable(white_triangle white_triangle.cpp)
example_executable(rotating_cube rotating_cube.cpp)
example_executable(targeting_camera targeting_camera.cpp)
example_executable(imgui imgui.cpp)

# OpenGLApp_imgui needs imgui.
find_package(imgui REQUIRED)
target_link_libraries(${PROJECT_NAME}_imgui PRIVATE imgui::imgui)

# Copy shader files to executable folder.
add_custom_target(copy_shaders COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_shaders.cmake)
add_dependencies(${PROJECT_NAME} copy_shaders)
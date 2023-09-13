function(example_executable target_name)
    SET(executable_name ${PROJECT_NAME}_${target_name})
    add_executable(${executable_name} ${target_name}/main.cpp)
    target_compile_features(${executable_name} PRIVATE cxx_std_20)
    target_link_libraries(${executable_name} PRIVATE OpenGLApp ${ARGN})
endfunction()

find_package(imgui REQUIRED)

example_executable(white_triangle)
example_executable(rotating_cube)
example_executable(targeting_camera)
example_executable(imgui imgui::imgui) # needs imgui library.
example_executable(framebuffer)

# Copy shader files to executable folder.
add_custom_target(copy_assets COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake)
add_dependencies(${PROJECT_NAME} copy_assets)
cmake_minimum_required(VERSION 3.0)
project(skeleton)

set(SKELETON_SRC_FILES
  src/skeleton/main.cc
)
source_group("skeleton/src" FILES ${SKELETON_SRC_FILES})

set(SKELETON_SRC_SCENE_FILES
  src/skeleton/scene/scene.hh src/skeleton/scene/scene.cc
)
source_group("skeleton/src/scene" FILES ${SKELETON_SRC_SCENE_FILES})

set(SKELETON_SRC_SHADER_FILES
  src/skeleton/shaders/pbr_first_pass.vert.glsl
  src/skeleton/shaders/pbr_first_pass.frag.glsl
  src/skeleton/shaders/pbr_post_process.vert.glsl
  src/skeleton/shaders/pbr_post_process.frag.glsl
)
source_group("skeleton/src/shaders" FILES ${SKELETON_SRC_SHADER_FILES})

add_executable(${PROJECT_NAME}
  ${SKELETON_SRC_FILES}
  ${SKELETON_SRC_SCENE_FILES}
  ${SKELETON_SRC_SHADER_FILES}
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC src)

add_subdirectory(src/depend/delta)
target_link_libraries(${PROJECT_NAME} PRIVATE delta)

if (PROJECT_IS_TOP_LEVEL)
  if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_GENERATE_SCHEME TRUE XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
    endif()
  if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif()
endif()

# Add custom target - compile glsl shaders to spirv
set(GLSLANG_VALIDATOR "glslangValidator")
file(GLOB_RECURSE GLSL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/skeleton/shaders/*.glsl")
foreach(GLSL_FILE ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL_FILE} NAME)
  string(REGEX REPLACE "[.]glsl$" ".spv" SPIRV_FILE_NAME ${FILE_NAME})
  set(SPIRV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/spv/${SPIRV_FILE_NAME}")
  add_custom_command(
    OUTPUT ${SPIRV_FILE}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/spv/"
    COMMAND ${GLSLANG_VALIDATOR} -V ${GLSL_FILE} -o ${SPIRV_FILE}
    DEPENDS ${GLSL_FILE})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV_FILE})
endforeach(GLSL_FILE)
add_custom_target(skeleton_shader_target DEPENDS ${SPIRV_BINARY_FILES})
add_dependencies(${PROJECT_NAME} skeleton_shader_target)

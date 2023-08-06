cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall")

file(GLOB_RECURSE EZ_SOURCES CONFIGURE_DEPENDS "src/ez/*.cpp")

add_library(ezUI ${EZ_SOURCES})

#target_compile_definitions(ezUI PUBLIC EZ_BUILD_ENABLE_PROFILING)
target_compile_definitions(ezUI PUBLIC EZ_BUILD_ENABLE_ASSERTIONS)
target_compile_definitions(ezUI PUBLIC EZ_BUILD_DEBUG_MODE)


target_include_directories(ezUI SYSTEM PRIVATE "vendor/glm" "vendor/glad" "vendor/stb" "vendor/glfw/include" )
target_include_directories(ezUI PUBLIC "src")

add_subdirectory(vendor/glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(ezUI OpenGL::GL glfw)

#add_executable(${PROJECT_NAME} ${EZ_SOURCES})
#add_compile_definitions(ZC_PLATFORM_LINUX DEBUG)
#target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "/opt/OpenZDK/ZUNE/Include/ZDK" "/opt/OpenZDK/NVIDIA/include")
#file(COPY src/assets DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
#target_link_libraries(${PROJECT_NAME} zdksystem.lib compclient.lib libEGL.lib)

cmake_minimum_required(VERSION 3.1)
project(Principia)

set(PROJ_BINARY_DIR  "bin")
set(PROJ_LIBRARY_DIR "lib")
#set(PROJ_INCLUDE_DIR "include")

set(CMAKE_CXX_STANDARD 17)

include_directories(
    lib/glm/glm
    lib/stb
)


include_directories(assets lib/glm/glm lib/stb)

add_subdirectory(src)

target_link_libraries(engine_lib PUBLIC lib/glm/glm)

#add_library(engine_lib INTERFACE src/Utility/mouse.h src/Utility/key.h)

add_executable(test tests/test_main.cpp)
target_link_libraries(test engine_lib)
#target_link_libraries(test engine_lib Rendering)
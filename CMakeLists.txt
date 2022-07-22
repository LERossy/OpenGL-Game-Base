cmake_minimum_required(VERSION 2.8)
project(opengl_w)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)


include_directories(${OPENGL_INCLUDE_DIR} glfw /home/aelhossini/Laila/OpenGL/glad/include /home/aelhossini/Laila/OpenGL/freetype-2.4.0/include ${ASSIMP_INCLUDE_DIR})

add_executable(image Image_Generator.cpp /home/aelhossini/Laila/OpenGL/glad/src/glad.c Shader.cpp Texture.cpp Resource_Manager.cpp Scene.cpp Object_Rend.cpp Camera.h stb_image.cpp)


target_link_libraries(image ${OPENGL_gl_LIBRARY} glfw dl)


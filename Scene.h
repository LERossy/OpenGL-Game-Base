#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <stdlib.h>

#include "Resource_Manager.h"
//#include "Post_Processor.h"
//#include "Text_Renderer.h"
#include "Object_Rend.h"
//#include "Icon_Renderer.h"
#include "Camera.h"

// Game class. General framework to potentially use for other games in the future.

struct Beam {
	glm::vec3 Position;
	glm::vec3 Size;
	float Rotation;
	glm::vec3 Axis;
};
	


class Scene
{
    public:
        // game state
		bool           Keys[1024];
        bool 		   KeysProcessed[1024];
        unsigned int Width, Height;
        double wRatio, hRatio;
        GLFWwindow* window;
        
        
        // class objects
        Camera *camera;
        ObjectRenderer *Cubes;
        //IconRenderer *Icons;
        
        //Model *Model;
        
        // object state
        std::vector <Beam> Beams;
        std::vector <glm::vec3> lightPos;
        
        // constructor/destructor
        Scene(GLFWwindow* window, unsigned int width, unsigned int height);
        ~Scene();
        // initialize game state (load all shaders/textures/levels)
        void Init();
        // game loop
        
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
};

#endif

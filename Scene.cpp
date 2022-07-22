#include "Scene.h"


/*
// collision functions
// -------------------
bool CheckCollision(SceneObject &one, SceneObject &two); // AABB - AABB collision
Collision CheckCollision(BallObject &one, SceneObject &two); // AABB - Circle collision
Direction VectorDirection(glm::vec2 target); 


// Mouse functions
// ---------------
bool CheckHover(double mousePosX, double mousePosY, Button &button);
*/


// Scene class functions
// --------------------
Scene::Scene(GLFWwindow* window, unsigned int width, unsigned int height) : Keys(), Width(width), Height(height), window(window)
{ 
	int winWidth = 0;
	int winHeight = 0;
	glfwGetWindowSize(window, &winWidth, &winHeight);
	if (winWidth > 0 && winHeight > 0)
	{
		this->wRatio = static_cast<float>(width) / winWidth;
		this->hRatio = static_cast<float>(height) / winHeight;
	}
	else
	{
		this->wRatio = 1;
		this->hRatio = 1;
	}
	this->camera = new Camera(glm::vec3(10.0f, 10.0f, 10.0f));
}

Scene::~Scene()
{

}

void Scene::Init()
{
	// load shaders
	ResourceManager::LoadShader("Shaders/Object_vShader.txt", "Shaders/Object_fShader.txt", nullptr, "objectShader");
	//ResourceManager::LoadShader("Shaders/Icon_vShader.txt", "Shaders/Icon_fShader.txt", nullptr, "iconShader");
    
    // configure shaders


    // set render-specific controls
    Cubes = new ObjectRenderer(ResourceManager::GetShader("objectShader"));
    //Icons = new IconRenderer(ResourceManager::GetShader("iconShader"));
    
    // load textures
	ResourceManager::LoadTexture("Textures/wood.png", false, "wood");
	//ResourceManager::LoadTexture("Textures/Center.png", true, "cross");
	
	//Basic Cubes
	Beam block1;
	block1.Position = glm::vec3(-5.0f, 0.0f, 0.0f);
	block1.Size =  glm::vec3(1.0f);
	block1.Rotation = 0.0f;
	block1.Axis = glm::vec3(0.0f, 0.0f, 1.0f);
	this->Beams.push_back(block1);
	
	Beam block2;
	block2.Position = glm::vec3(0.0f, 0.0f, -5.0f);
	block2.Size = glm::vec3(1.0f);
	block2.Rotation = 0.0f;
	block2.Axis = glm::vec3(0.0f, 0.0f, 1.0f);
	this->Beams.push_back(block2);
	
	Beam block3;
	block3.Position = glm::vec3(0.0f, 5.0f, 0.0f);
	block3.Size = glm::vec3(1.0f);
	block3.Rotation = 0.0f;
	block3.Axis = glm::vec3(0.0f, 0.0f, 1.0f);
	this->Beams.push_back(block3);
	
	Beam block4;
	block4.Position = glm::vec3(0.0f, -5.0f, 0.0f);
	block4.Size = glm::vec3(1.0f);
	block4.Rotation = 0.0f;
	block4.Axis = glm::vec3(0.0f, 0.0f, 1.0f);
	this->Beams.push_back(block4);
	
	Beam block5;
	block5.Position = glm::vec3(0.0f, 0.0f, 5.0f);
	block5.Size = glm::vec3(1.0f);
	block5.Rotation = 0.0f;
	block5.Axis = glm::vec3(0.0f, 0.0f, 1.0f);
	this->Beams.push_back(block5);
	
	Beam block6;
	block6.Position = glm::vec3(5.0f, 0.0f, 0.0f);
	block6.Size = glm::vec3(1.0f);
	block6.Rotation = 0.0f;
	block6.Axis = glm::vec3(0.0f, 0.0f, 1.0f);
	this->Beams.push_back(block6);
	
	
	//light Postions
	lightPos.push_back(glm::vec3(-5.0f, 2.0f, 0.0f));
	lightPos.push_back(glm::vec3(5.0f, 2.0f, 0.0f));
	lightPos.push_back(glm::vec3(0.0f, 2.0f, -5.0f));
	lightPos.push_back(glm::vec3(0.0f, 2.0f, 5.0f));
	
	
	//Linkages
	crossLinkage(0, 2, glm::vec3(-5.0f, 0.0f, 0.0f), 90.0f, 1);
	
}


void Scene::ProcessInput(float dt)
{
    if (this->Keys[GLFW_KEY_W])
    {
    	this->camera->ProcessKeyboard(FORWARD, dt);
    	//std::cout << this->camera->Position.x << " " << this->camera->Position.y << " " << this->camera->Position.z << std::endl;
    }
    if (this->Keys[GLFW_KEY_S])
    {
    	this->camera->ProcessKeyboard(BACKWARD, dt);
    	//std::cout << this->camera->Position.x << " " << this->camera->Position.y << " " << this->camera->Position.z << std::endl;
    }
    if (this->Keys[GLFW_KEY_A])
    {
    	this->camera->ProcessKeyboard(LEFT, dt);
    	//std::cout << this->camera->Position.x << " " << this->camera->Position.y << " " << this->camera->Position.z << std::endl;
    }
    if (this->Keys[GLFW_KEY_D])
    {
    	this->camera->ProcessKeyboard(RIGHT, dt);
    	//std::cout << this->camera->Position.x << " " << this->camera->Position.y << " " << this->camera->Position.z << std::endl;
    }
}


void Scene::Update(float dt)
{
	// update objects
	// --------------
	// update camera position
	ResourceManager::GetShader("objectShader").Use();
    float fov = this->camera->fov;
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)this->Width / (float)this->Height, 0.1f, 100.0f);
    glm::mat4 view = this->camera->GetViewMatrix();
    ResourceManager::GetShader("objectShader").SetMatrix4("projection", projection);
    ResourceManager::GetShader("objectShader").SetMatrix4("view", view);
    ResourceManager::GetShader("objectShader").SetVector3f("viewPos", this->camera->Position);
	ResourceManager::GetShader("objectShader").SetInteger("nr_point_lights", this->lightPos.size());
	
	char buf[30];	
    for (int i = 0; i < this->lightPos.size(); i++)
	{
		sprintf(buf, "lightPositions[%d]", i);
		ResourceManager::GetShader("objectShader").SetVector3f(buf, this->lightPos[i]);
	}
	
	
	//this->camera->UpdatePosition(Collided, dt);
}

void Scene::Render()
{
	auto wood = ResourceManager::GetTexture("wood");
	//auto cross = ResourceManager::GetTexture("cross");
	
	// Set up post processing framebuffers
	// -----------------------------------
	//Effects->BeginRender();
	
	// Render scene
	// ------------
    // draw background

    // draw objects
    for (int i = 0; i < this->Beams.size(); i++)
    {
		this->Cubes->DrawCube(wood, this->Beams[i].Position, this->Beams[i].Size, this->Beams[i].Rotation, this->Beams[i].Axis, false, glm::vec3(0.5f, 0.8f, 0.0f));
		//std::cout << "Drawn" << std::endl;
	}
	
	this->Cubes->DrawSphere(glm::vec3(5.0f, 2.5f, 0.0f), 0.1f, glm::vec3(0.0f, 1.0f, 1.0f), true);
	this->Cubes->DrawSphere(lightPos[0], 0.1f, glm::vec3(1.0f, 1.0f, 1.0f), true);
	this->Cubes->DrawSphere(lightPos[1], 0.1f, glm::vec3(1.0f, 1.0f, 1.0f), true);
	this->Cubes->DrawSphere(lightPos[2], 0.1f, glm::vec3(1.0f, 1.0f, 1.0f), true);
	this->Cubes->DrawSphere(lightPos[3], 0.1f, glm::vec3(1.0f, 1.0f, 1.0f), true);
	
    
    // Render Text

    
    // Do post processing effects
    // --------------------------
    /*
    ResourceManager::GetShader("iconShader").Use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("iconShader").SetMatrix4("projection", projection);
    Icons->DrawIcon(cross, glm::vec2(this->Width/2.0, this->Height/2.0), glm::vec2(10.0f, 10.0f));
    */
}
        
        
        
        
        
        
        


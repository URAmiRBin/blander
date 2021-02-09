#pragma once
#include"libs.h"



class Application
{
private:
	// Variables
	GLFWwindow* window;
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = WINDOW_WIDTH;
	int framebufferHeight = WINDOW_HEIGHT;

	bool freelook = true;
	int selected = 0;
	int currentTexture = 0;
	
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	
	float FOV;
	float clipDistance;
	float drawDistance;
	float aspectRatio;

	float delta;
	float now;
	float before;

	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool mouseFirst;

	Camera camera;

	std::vector<Shader*> shaders;
	std::vector<Texture*> textures;
	std::vector<Material*> materials;
	std::vector<Mesh*> meshes;
	std::vector<Mesh*> grid;
	std::vector<glm::vec3*> lights;


	// Functions
	void initGlfw();
	void initWindow(const char* title, bool resizable);
	void initGlew();
	void initOpengl();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initMeshes();
	void initLights();
	void initUniforms();
	void updateUniforms();
public:
	// Functions

	Application(const char* title,
		const int width, const int height, bool resizable);
	
	virtual ~Application();

	void addObject(int type);
	int getWindowShouldClose();
	void setWindowShouldClose();
	void updateDelta();
	void updateMouseInput();
	void updateKeyboardInput();
	void update();
	void render();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight);
};


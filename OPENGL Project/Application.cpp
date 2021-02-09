#include "Application.h"

/* ========================= CONSTRUCTOR DESTRUCTOR =========================== */
// Constructor
Application::Application(const char* title,
	const int width, const int height, bool resizable)
	: WINDOW_WIDTH(width), WINDOW_HEIGHT(height),
	camera(glm::vec3(0.f, 0.f, 2.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f))
{
	this->window = nullptr;
	this->framebufferHeight = this->WINDOW_HEIGHT;
	this->framebufferWidth = this->WINDOW_WIDTH;


	// Camera and Perspective settings
	this->FOV = 75.f;
	this->clipDistance = 0.1f;
	this->drawDistance = 1000.f;
	this->aspectRatio = static_cast<float>(framebufferWidth / framebufferHeight);

	// Default Time
	this->delta = 0.f;
	this->now = 0.f;
	this->before = 0.f;

	// Default Mouse settings
	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->mouseFirst = true;


	// Initialize
	this->initGlfw();
	this->initWindow(title, resizable);
	this->initGlew();
	this->initOpengl();
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initMeshes();
	this->initLights();
	this->initUniforms();
}

// Destructor
Application::~Application() {
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for (size_t i = 0; i < this->shaders.size(); i++)
	{
		delete this->shaders[i];
	}

	for (size_t i = 0; i < this->materials.size(); i++)
	{
		delete this->materials[i];
	}

	for (size_t i = 0; i < this->textures.size(); i++)
	{
		delete this->textures[i];
	}

	for (size_t i = 0; i < this->meshes.size(); i++)
	{
		delete this->meshes[i];
	}

	for (size_t i = 0; i < this->grid.size(); i++)
	{
		delete this->grid[i];
	}

	for (size_t i = 0; i < this->lights.size(); i++)
	{
		delete this->lights[i];
	}
}



/* ========================= INITIALIZE OPENGL AND THE GUYS =========================== */
// Initialize Window
void Application::initWindow(const char* title, bool resizable) {
	// Window Settings
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	// Create Window
	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);
	
	// Check Errors
	if (this->window == nullptr) {
		std::cout << "ERROR : Application::InitWindow - Can not create window" << std::endl;
		glfwTerminate();
	}
	
	// Save framebuffer size
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	
	// Keyboard and Resize callback
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(this->window, &key_callback); 
	glfwSetFramebufferSizeCallback(this->window, Application::framebuffer_resize_callback);

	// Set Active window
	glfwMakeContextCurrent(this->window);

	// Set Swap interval time
	// WARNING: COMMENT TO UNLEASH THE TRUE POWER OF YOUR GRAPHICS CARD
	//glfwSwapInterval(1);

}

// Initialize GLFW
void Application::initGlfw() {
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "ERROR : Application::InitGlfw - Can not initialize GLFW" << std::endl;
		glfwTerminate();
	}
}

// Initialize GLEW
void Application::initGlew() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR : Application::InitGlew - Can not initialize GLEW" << std::endl;
		glfwTerminate();
	}
}

// Set OPENGL Options
void Application::initOpengl() {
	// Perspective View
	glEnable(GL_DEPTH_TEST);
	
	// Single Face Tri
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	// Blending Color Options
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set Polygon Mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Active Mouse input
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


/* ========================= INITIALIZE TEXTURES, SHADERS, MESHES AND THE GUYS =========================== */
// Initialize Default View and Projection Matrices
void Application::initMatrices() {
	this->ViewMatrix = glm::mat4(1.f);
	
	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(glm::radians(this->FOV), this->aspectRatio, this->clipDistance, this->drawDistance);

}

// Initialize Shader Programs From files
void Application::initShaders()
{
	this->shaders.push_back(new Shader("vertex_core.glsl", "fragment_core.glsl"));
	this->shaders.push_back(new Shader("vertex_core.glsl", "fragment_core2.glsl"));
}

// Initialize Textures From files
void Application::initTextures()
{
	// IMPORTANT : First load the texture and then the speculat map of it
	this->textures.push_back(new Texture("Images/wood.jpg", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/woods.jpg", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/blue.jpg", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/blue.jpg", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/metal.jpg", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/metalS.jpg", GL_TEXTURE_2D));
}

// Initialize Materials
void Application::initMaterials()
{
	/* Inputs of Material
	Ambient Light Intensity, Diffuse Light Intensity, Specular Light Intensity,
	Diffuse Texture Unit, Specular Texture Unit */
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(0.5f), 0, 1));
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(0.5f), 2, 3));
}

// Initialize Meshes and Grid
void Application::initMeshes()
{
	Quad quad = Quad();
	/* Input of Mesh
	Primitive, Diffuse Texture, Specular Texture, Material*/
	this->meshes.push_back(new Mesh(&quad, this->textures[2], this->textures[3], this->materials[0]));
	
	// Initialize Floor Grid
	int i;
	for (i = 0; i < 30; i++) {
		Line line = Line(glm::vec3((i - 15) * 0.5f, -1.f, -7.5f), glm::vec3((i - 15) * 0.5f, -1.f, 7.5f));
		this->grid.push_back(new Mesh(&line, this->textures[2], this->textures[3], this->materials[0]));
	}
	for (i = 0; i < 30; i++) {
		Line line = Line(glm::vec3(-7.5f, -1.f, (i - 15) * 0.5f), glm::vec3(7.5f, -1.f, (i - 15) * 0.5f));
		this->grid.push_back(new Mesh(&line, this->textures[2], this->textures[3], this->materials[0]));
	}
}

// Initialize Light
void Application::initLights()
{
	// Input: Position of source
	this->lights.push_back(new glm::vec3(0.f, 0.f, 5.f));
}

// Initialize Uniforms
void Application::initUniforms()
{
	// Send Uniforms to default shader program
	int j;
	for (j = 0; j < shaders.size(); j++) {
		this->shaders[j]->setMat4fv(this->ViewMatrix, "ViewMatrix");
		this->shaders[j]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
		this->shaders[j]->setVec3f(*this->lights[0], "lightPos0");
	}
}

/* ################################## MAIN WHILE LOOP #################################### */
/* ========================= UPDATE FUNCTIONS =========================== */
// Update Body
void Application::update() {
	this->updateDelta();
	glfwPollEvents();
	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(this->delta, -1, this->mouseOffsetX, this->mouseOffsetY);
}

// Update Uniforms Changed with Input
void Application::updateUniforms() {
	// Camera : Move and Rotate
	this->ViewMatrix = this->camera.getViewMatrix();
	;
	// Projection Matrix : Resize Frame Buffer
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	this->aspectRatio = static_cast<float>(this->framebufferWidth / this->framebufferHeight);
	
	this->ProjectionMatrix = glm::perspective(glm::radians(this->FOV), this->aspectRatio, this->clipDistance, this->drawDistance);
	
	int j;
	for (j = 0; j < shaders.size(); j++) {
		this->shaders[j]->setMat4fv(this->ViewMatrix, "ViewMatrix");
		this->shaders[j]->setVec3f(this->camera.getPosition(), "camPos");
		this->shaders[j]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
	}
}

// Keyboard Input Actions
void Application::updateKeyboardInput()
{
	// ESC : Close App
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		this->setWindowShouldClose();
	}

	// WASD : Camera Movement
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
		this->camera.updateKeyboardInput(this->delta, 0);
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		this->camera.updateKeyboardInput(this->delta, 1);
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		this->camera.updateKeyboardInput(this->delta, 2);
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		this->camera.updateKeyboardInput(this->delta, 3);
	}
	if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS) {
		this->camera.updateKeyboardInput(this->delta, 6);
	}
	if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS) {
		this->camera.updateKeyboardInput(this->delta, 7);
	}

	if (!this->freelook) {
		// Move Selected Mesh with Camera in edit mode
		this->meshes[this->selected]->setPosition(this->camera.getPosition() + this->camera.getFront() + this->camera.getFront());

		if (glfwGetKey(this->window, GLFW_KEY_H) == GLFW_PRESS) {
			this->meshes[selected]->rotateIt(glm::vec3(0.f, 1.f, 0.f));
		}
		if (glfwGetKey(this->window, GLFW_KEY_F) == GLFW_PRESS) {
			this->meshes[selected]->rotateIt(glm::vec3(0.f, -1.0f, 0.f));
		}
		if (glfwGetKey(this->window, GLFW_KEY_T) == GLFW_PRESS) {
			this->meshes[selected]->rotateIt(glm::vec3(1.f, 0.f, 0.f));
		}
		if (glfwGetKey(this->window, GLFW_KEY_B) == GLFW_PRESS) {
			this->meshes[selected]->rotateIt(glm::vec3(-1.f, 0.f, 0.f));
		}
		if (glfwGetKey(this->window, GLFW_KEY_Y) == GLFW_PRESS) {
			this->meshes[selected]->rotateIt(glm::vec3(0.f, 0.f, 1.f));
		}
		if (glfwGetKey(this->window, GLFW_KEY_R) == GLFW_PRESS) {
			this->meshes[selected]->rotateIt(glm::vec3(0.f, 0.0f, -1.f));
		}

		if (glfwGetKey(this->window, GLFW_KEY_L) == GLFW_PRESS) {
			this->meshes[selected]->scaleIt(glm::vec3(0.01f, 0.f, 0.f));
		}
		if (glfwGetKey(this->window, GLFW_KEY_J) == GLFW_PRESS) {
			this->meshes[selected]->scaleIt(glm::vec3(-0.01f, 0.f, 0.f));
		}
		if (glfwGetKey(this->window, GLFW_KEY_I) == GLFW_PRESS) {
			this->meshes[selected]->scaleIt(glm::vec3(0.f, 0.01f, 0.f));
		}
		if (glfwGetKey(this->window, GLFW_KEY_M) == GLFW_PRESS) {
			this->meshes[selected]->scaleIt(glm::vec3(0.f, -0.01f, 0.f));
		}
		if (glfwGetKey(this->window, GLFW_KEY_O) == GLFW_PRESS) {
			this->meshes[selected]->scaleIt(glm::vec3(0.f, 0.f, 0.01f));
		}
		if (glfwGetKey(this->window, GLFW_KEY_U) == GLFW_PRESS) {
			this->meshes[selected]->scaleIt(glm::vec3(0.f, 0.f, -0.01f));
		}
	}

}

// Update delta time for FPS and Speed of camera
void Application::updateDelta()
{
	float fps;
	this->now = static_cast<float>(glfwGetTime());
	this->delta = this->now - this->before;
	this->before = this->now;
	fps = 1 / delta;
	std::cout << "Framerate : " << fps << " fps" << std::endl;
}

// Mouse Input
void Application::updateMouseInput() {
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->mouseFirst) {
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->mouseFirst = false;
	}

	// Offset for rotating the camera
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;
}

// Add new Object
void Application::addObject(int type)
{
	if (type == GLFW_KEY_C) {
		Cube c = Cube();
		this->meshes.push_back(new Mesh(&c, this->textures[0], this->textures[1], this->materials[0]));
		this->meshes[this->meshes.size() - 1]->setPosition(this->camera.getPosition() + this->camera.getFront() + this->camera.getFront());
	}
	else if (type == GLFW_KEY_V) {
		Prism c = Prism();
		this->meshes.push_back(new Mesh(&c, this->textures[0], this->textures[1], this->materials[0]));
		this->meshes[this->meshes.size() - 1]->setPosition(this->camera.getPosition() + this->camera.getFront() + this->camera.getFront());
	}
	else if (type == GLFW_KEY_B) {
		Pyramid c = Pyramid();
		this->meshes.push_back(new Mesh(&c, this->textures[0], this->textures[1], this->materials[0]));
		this->meshes[this->meshes.size() - 1]->setPosition(this->camera.getPosition() + this->camera.getFront() + this->camera.getFront());
	}
}

/* ========================= RENDER =========================== */
void Application::render() {
	// Clear + Dark Sky
	glClearColor(0.0f, 0.03f, 0.1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	// Update changed uniforms with keyboard input
	this->updateUniforms();

	// Activate default material on default shader program
	int j;
	for (j = 0; j < shaders.size(); j++) {
		this->materials[j]->sendToShader(*this->shaders[j]);
	}
	
	// Use default shader program
	this->shaders[1]->use();
	
	// Render Grid
	int i;
	for (i = 0; i < 60; i++) {
		this->grid[i]->render(this->shaders[0]);
	}

	// Render Meshes
	// In Edit mode wireframe unselected meshes
	if (!this->freelook) {
		this->meshes[this->selected]->render(this->shaders[1]);
	}
	for (i = 0; i < this->meshes.size(); i++) {
		if(i != this->selected || this->freelook)
			this->meshes[i]->render(this->shaders[0]);
	}

	// Reset settings
	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/* ========================= CALLBACK FUNCTIONS =========================== */
// Resize Callback
void Application::framebuffer_resize_callback(GLFWwindow* window, int framebufferWidth, int framebufferHeight) {
	glViewport(0, 0, framebufferWidth, framebufferHeight);
};

// Keyboard Callback
void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app->freelook) {
		if (key > 47 && key < 58 && action == GLFW_PRESS)
		{
			app->selected = key - 48;
		}
		else if ((key == GLFW_KEY_C || key == GLFW_KEY_B || key == GLFW_KEY_V) && action == GLFW_PRESS) {
			app->addObject(key);
		}
	}
	else {
		if (key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
			app->meshes[app->selected]->changeTexture(app->textures[app->currentTexture * 2], app->textures[app->currentTexture * 2 + 1]);
			app->currentTexture = (app->currentTexture + 1) % (app->textures.size() / 2);
		}
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		app->freelook = !app->freelook;
		if (app->freelook) {
			std::cout << "Changed mode to Freelook\n";
		}
		else {
			if (app->selected < app->meshes.size()) {
				glm::vec3 newposition = app->meshes[app->selected]->getPosition() - app->camera.getFront() - app->camera.getFront();
				app->camera.setCameraPosition(newposition);
				std::cout << "Changed mode to Edit: Editing Object " << app->selected << std::endl;
			}
			else {
				std::cout << "Changed mode to Edit: Editing No Object" << std::endl;
			}
		}
	}
	if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
		app->camera.updateKeyboardInput(app->delta, 4);
		app->camera.resetFront();
	}
	if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
		app->camera.updateKeyboardInput(app->delta, 5);
		app->camera.resetFront();
	}
}


/* ========================= OTHER FUNCTIONS =========================== */
// Window status
int Application::getWindowShouldClose() {
	return glfwWindowShouldClose(this->window);
}

// Close window
void Application::setWindowShouldClose() {
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int scrHeight = 800;
const int scrWidth	= 600;

const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor\n"
	"void main() {\n"
	" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

void framebuffer_size_callback(GLFWwindow* window, int height, int width);
void processInput(GLFWwindow* window);

int main(void) {
	// initialize glfw version and profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create glfw window
	GLFWwindow* window = glfwCreateWindow(scrHeight, scrWidth, "LearnOpenGL", NULL, NULL);
	if (window == nullptr) {
		std::cerr << "Failed to initialize GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // resizes viewport when user changes window size

	// set up glad pointer
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	/////////////////
	////// VBO //////
	/////////////////

	// simple triangle vertex
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 	
		 0.5f, -0.5f, 0.0f, 	
		 0.0f,  0.5f, 0.0f, 	
	};

	// create buffer object and bind it to GL_ARRAY_BUFFER
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// copy custom vertex to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	///////////////////////////
	////// VERTEX SHADER //////
	///////////////////////////

	// create, attach, and compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	// error check vertex shader compilation
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex shader was unable to compile properly\n"
			<< infoLog << std::endl;
		return -1;
	}


	/////////////////////////////
	////// FRAGMENT SHADER //////
	/////////////////////////////

	// create, attach, and compile fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// error check fragment shader compilation
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Fragment shader was unable to compile properly\n"
			<< infoLog << std::endl;
		return -1;
	}


	////////////////////////////
	////// SHADER PROGRAM //////
	////////////////////////////

	// create shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// attach and link vertex and fragment shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check shader program's compilation status
	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "Shader program was unable to compile properly\n"
			<< infoLog << std::endl;
		return -1;
	}

	// use shader program and clean-up individual shaders
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//////////////////
	///// RENDER /////
	//////////////////

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// render settings
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int height, int width) {
	glViewport(0, 0, height, width);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
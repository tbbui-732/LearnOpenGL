#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

const int scrHeight = 800;
const int scrWidth	= 600;
const int LOG_SZ	= 512;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSourceOrange = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShaderSourceYellow = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";



void framebuffer_size_callback(GLFWwindow* window, int height, int width);
void processInput(GLFWwindow* window);
bool programCompiled(unsigned int& shader, const char* shaderName, bool isShaderProgram);

int main(void) {
	/////////////////////////
	////// GLFW & GLAD //////
	/////////////////////////
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

	//////////////////////
	////// VERTICES //////
	//////////////////////
	float firstTriangleVertices[] = {
		-0.75f, 0.9f,  0.0f,
		-0.75f, 0.25f, 0.0f, 	
		-0.25f, 0.9f,  0.0f, 	
	};

	float secondTriangleVertices[] = {
		0.75f, -0.9f,  0.0f,
		0.75f, -0.25f, 0.0f,
		0.25f, -0.9f,  0.0f
	};

	/////////////////
	////// VAO //////
	/////////////////
	unsigned int firstTriangleVAO, secondTriangleVAO;
	glGenVertexArrays(1, &firstTriangleVAO);
	glGenVertexArrays(1, &secondTriangleVAO);
	glBindVertexArray(firstTriangleVAO);
	glBindVertexArray(secondTriangleVAO);


	/////////////////
	////// VBO //////
	/////////////////
	// Create buffer objects
	unsigned int firstTriangleVBO, secondTriangleVBO;
	glGenBuffers(1, &firstTriangleVBO);
	glGenBuffers(1, &secondTriangleVBO);

	// Setting up first triangle
	glBindVertexArray(firstTriangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, firstTriangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangleVertices), firstTriangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// Setting up second triangle
	glBindVertexArray(secondTriangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, secondTriangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangleVertices), secondTriangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);


	///////////////////////////
	////// VERTEX SHADER //////
	///////////////////////////
	// create, attach, and compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	// error check vertex shader compilation
	if (!programCompiled(vertexShader, "Vertex shader", false)) {
		exit(1);
	}


	/////////////////////////////
	////// FRAGMENT SHADER //////
	/////////////////////////////
	// create, attach, and compile fragment shader
	unsigned int fragmentShaderOrange, fragmentShaderYellow;
	fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSourceOrange, NULL);
	glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
	glCompileShader(fragmentShaderOrange);
	if (!programCompiled(fragmentShaderOrange, "Fragment shader", false)) {
		exit(1);
	}
	glCompileShader(fragmentShaderYellow);
	if (!programCompiled(fragmentShaderYellow, "Fragment shader", false)) {
		exit(1);
	}


	////////////////////////////
	////// SHADER PROGRAM //////
	////////////////////////////
	// create shader program
	unsigned int orangeProgram, yellowProgram;
	orangeProgram = glCreateProgram();
	yellowProgram = glCreateProgram();

	// attach and link vertex and fragment shaders
	glAttachShader(orangeProgram, vertexShader);
	glAttachShader(orangeProgram, fragmentShaderOrange);
	glLinkProgram(orangeProgram);
	if (!programCompiled(orangeProgram, "Shader program", true)) {
		exit(1);
	}

	glAttachShader(yellowProgram, vertexShader);
	glAttachShader(yellowProgram, fragmentShaderYellow);
	glLinkProgram(yellowProgram);
	if (!programCompiled(yellowProgram, "Shader program", true)) {
		exit(1);
	}

	// clean-up individual shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderOrange);
	glDeleteShader(fragmentShaderYellow);


	//////////////////
	///// RENDER /////
	//////////////////
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// background
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// draw first triangle (orange)
		glUseProgram(orangeProgram);
		glBindVertexArray(firstTriangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// draw second triangle (yellow)
		glUseProgram(yellowProgram);
		glBindVertexArray(secondTriangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocate everything once program terminates
	glDeleteVertexArrays(1, &firstTriangleVAO);
	glDeleteVertexArrays(1, &secondTriangleVAO);
	glDeleteBuffers(1, &firstTriangleVBO);
	glDeleteBuffers(1, &secondTriangleVBO);
	glDeleteProgram(orangeProgram);
	glDeleteProgram(yellowProgram);
	
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

bool programCompiled(unsigned int& shader, const char* shaderName, bool isShaderProgram) {
	int success;
	char log[LOG_SZ];
	if (!isShaderProgram) { // checks shaders
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, LOG_SZ, NULL, log);
			std::cerr << shaderName << " was unable to compile properly\n"
				<< log << std::endl;
			return false;
		}
	}
	else { // check shader programs
		glGetProgramiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, LOG_SZ, NULL, log);
			std::cerr << shaderName << " was unable to compile properly\n"
				<< log << std::endl;
			return false;
		}
	}
	return true;
}

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

const char* fragmentShaderSource = "#version 330 core\n"
"uniform vec4 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = ourColor;\n"
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

	////////////////////
	///// VERTICES /////
	////////////////////
	float triangleVertices[] = {
		-0.5f, -0.5f, 0.0f, 	
		 0.5f, -0.5f, 0.0f, 	
		 0.0f,  0.5f, 0.0f, 	
	};

	///////////////
	///// VAO /////
	///////////////
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	///////////////
	///// VBO /////
	///////////////
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);


	//////////////////////
	///// ATTRIBUTES /////
	//////////////////////
	// vertex array contains positional data (x,y,z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);


	///////////////////
	///// SHADERS /////
	///////////////////
	// create individual shaders
	unsigned int vertexShader, fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// attach their sources
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// compile individual shaders
	glCompileShader(vertexShader);
	if (!programCompiled(vertexShader, "Vertex shader", false))
		exit(1);

	glCompileShader(fragmentShader);
	if (!programCompiled(fragmentShader, "Fragment shader", false))
		exit(1);

	// attach individual shaders into shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// link shader program
	glLinkProgram(shaderProgram);
	if (!programCompiled(shaderProgram, "Shader program", true)) {
		exit(1);
	}

	// clean-up/delete individual shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//////////////////
	///// RENDER /////
	//////////////////
	int uniformLocation = glGetUniformLocation(shaderProgram, "ourColor");
	float timeValue, greenValue;
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// background
		glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		timeValue = glfwGetTime();
		greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		glUniform4f(uniformLocation, 0.0f, greenValue, 0.0f, 1.0f); // continuously update the green value
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up buffers and shader program
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	
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

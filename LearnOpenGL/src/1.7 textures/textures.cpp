#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include "../../dependencies/include/learnopengl/shader.h"
#include "../../dependencies/include/stb_image/stb_image.h"

const int scrHeight = 800;
const int scrWidth	= 600;

const std::string shaderPath = std::filesystem::current_path().string() + "/src/1.7 textures/shaders/";

void framebuffer_size_callback(GLFWwindow* window, int height, int width);
void processInput(GLFWwindow* window);

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
		std::cout << "Failed to initialize GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // resizes viewport when user changes window size

	// set up glad pointer
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	////////////////////
	///// VERTICES /////
	////////////////////
	float triangleVertices[] = {
		//  position				colors
			-0.5f, -0.5f, 0.0f, 	1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 	0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f, 	0.0f, 0.0f, 1.0f,
	};


	////////////////////
	///// TEXTURES /////
	////////////////////
	// texture coordinates
	float texCoords[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.5f, 1.0f, 0.0f,
	};

	// texture wrapping; blocky when minifying and smooth when magnifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	///////////////////
	///// SHADERS /////
	///////////////////
	std::string vertPath, fragPath;
	vertPath = shaderPath + "shader.vs";
	fragPath = shaderPath + "shader.fs";
	Shader shaderProgram(vertPath.c_str(), fragPath.c_str());


	//////////////////
	///// RENDER /////
	//////////////////
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// background
		glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		shaderProgram.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up buffers and shader program
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
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

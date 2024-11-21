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
const std::string texturePath = std::filesystem::current_path().string() + "/resources/textures/";

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
	float vertices[] = {
		// positions		  // colors		      // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f	// top left
	};

	unsigned int rectIndices[] = {
		0,1,3, // first triangle
		1,2,3  // second triangle
	};


	//////////////////////
	///// ATTRIBUTES /////
	//////////////////////
	// position coords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	////////////////////
	///// TEXTURES /////
	////////////////////
	// generate texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load texture
	int texWidth, texHeight, texNChannels;
	std::string containerImage = texturePath + "container.jpg";
	unsigned char* data = stbi_load(containerImage.c_str(), &texWidth, &texHeight, &texNChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load: " << containerImage << std::endl;
	}

	// clean-up
	stbi_image_free(data);


	///////////////
	///// VAO /////
	///////////////
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	///////////////
	///// EBO /////
	///////////////
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices, GL_STATIC_DRAW);


	///////////////
	///// VBO /////
	///////////////
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


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

		// bind texture
		//glBindTexture(GL_TEXTURE_2D, texture);

		// draw triangle
		shaderProgram.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

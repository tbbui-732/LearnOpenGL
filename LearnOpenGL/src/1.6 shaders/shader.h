#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath) {
		// 1. retrieve source code from file paths
		std::string vertexCodeStr, fragmentCodeStr;
		std::ifstream vertexFile, fragmentFile;
		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			// open files
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);
			std::stringstream vertexStream, fragmentStream;

			// read file's buffer content into streams
			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();

			// close file handlers
			vertexFile.close();
			fragmentFile.close();

			// convert stream into string
			vertexCodeStr = vertexStream.str();
			fragmentCodeStr = fragmentStream.str();
		} 
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		const char* vertexShaderSource = vertexCodeStr.c_str();
		const char* fragmentShaderSource = fragmentCodeStr.c_str();

		// 2. compile and link shaders
		unsigned int vertex, fragment;
		int success;
		char log[512];

		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShaderSource, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, log);
			std::cerr << "ERROR::VERTEX::SHADER::COMPILATION_FAILED"
				<< log << std::endl;
		}

		// fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, log);
			std::cerr << "ERROR::FRAGMENT::SHADER::COMPILATION_FAILED"
				<< log << std::endl;
		}

		// 3. shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, log);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				<< log << std::endl;
		}

		// 4. clean up individual shaders
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// activates shader program
	void use(); 

	// queries uniform location and sets its value
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

#endif
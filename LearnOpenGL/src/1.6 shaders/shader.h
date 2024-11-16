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
		// retrieve source code from file paths
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

		const char* vertexShaderCode = vertexCodeStr.c_str();
		const char* fragmentShaderCode = fragmentCodeStr.c_str();
	}

	// activates shader program
	void use(); 

	// queries uniform location and sets its value
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

#endif
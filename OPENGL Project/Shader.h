#pragma once

#include<iostream>
#include<fstream>
#include<string>

#include<glew.h>
#include<glfw3.h>
#include<glm.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<mat4x4.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

class Shader {
private:
	// Variables
	// Shader Program ID
	GLuint id;

	// File Reader
	std::string loadShaderSource(const char* filename) {
		std::string temp = "";
		std::string src = "";

		std::ifstream in_file;

		in_file.open(filename);

		if (in_file.is_open()) {
			while (std::getline(in_file, temp)) {
				src += temp + "\n";
			}
		}
		else {
			std::cout << "ERROR : Shader::loadShaderSource - Can not open shader file" << filename << std::endl;
		}
		in_file.close();
		return src;
	}

	// Loads and returns Shader (Vertex or Fragment) from file
	GLuint loadShader(GLenum type,const char* filename) {
		char infoLog[512];
		GLint success;
		
		GLuint shader = glCreateShader(type);
		
		std::string str_src = this->loadShaderSource(filename);
		const GLchar* src = str_src.c_str();
		
		glShaderSource(shader, 1, &src, NULL);
		//Compiles Shader from filename
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR : Shader::loadShader - Can not compile shader " << filename << std::endl;
			std::cout << infoLog << std::endl;
		}

		return shader;
	}

	// Links given shaders to this shader program
	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader) {
		char infoLog[512];
		GLint success;

		this->id = glCreateProgram();

		glAttachShader(this->id, vertexShader);
		if (geometryShader) {
			glAttachShader(this->id, geometryShader);
		}
		glAttachShader(this->id, fragmentShader);

		glLinkProgram(this->id);



		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "ERROR : Shader::linkProgram - Can not link program" << std::endl;
			std::cout << infoLog << std::endl;
		}
		glUseProgram(0);

	}

public:
	// Constructor
	Shader(const char* vertexFile,const char* fragmentFile,const char* geometryFile = "") {
		GLuint vertexShader = 0;
		GLuint geometryShader = 0;
		GLuint fragmentShader = 0;

		// Load Vertex and Fragment Shaders
		vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
		fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

		// Load Geometry Shader if given any
		if (geometryFile != "") {
			geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);
		}

		// Links shaders to shader program
		this->linkProgram(vertexShader, geometryShader, fragmentShader);

		// Clean up
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
	}

	//Destructor
	~Shader() {
		glDeleteProgram(this->id);
	}

	// Use and unuse shader program
	void use() {
		glUseProgram(this->id);
	}

	void unuse() {
		glUseProgram(0);
	}


	// Send Uniforms to shader program
	void setVec4f(glm::fvec4 value, const GLchar* name) {
		this->use();
		glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void setVec3f(glm::fvec3 value, const GLchar* name) {
		this->use();
		glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void setVec2f(glm::fvec2 value, const GLchar* name) {
		this->use();
		glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
		this->unuse();
	}

	void setVec1f(GLfloat value, const GLchar* name) {
		this->use();
		glUniform1f(glGetUniformLocation(this->id, name), value);
		this->unuse();
	}

	void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE) {
		this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
		this->unuse();
	}

	void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE) {
		this->use();
		glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));
		this->unuse();
	}

	void set1i(GLint value, const GLchar* name) {
		this->use();
		glUniform1i(glGetUniformLocation(this->id, name), value);
		this->unuse();
	}
};
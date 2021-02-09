#pragma once


#include<glew.h>
#include<glfw3.h>
#include<glm.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<mat4x4.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

#include"Shader.h"


class Material {
private:
	// Variables
	// Light Intensity
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	// Texture units
	GLint diffuseTex;
	GLint specularTex;
public:
	// Constructor
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint diffuseTex, GLint specularTex) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}

	// Destructor
	~Material() {
		
	}

	// Unifrom Send to shader
	void sendToShader(Shader& programId) {
		programId.setVec3f(this->ambient, "material.ambient");
		programId.setVec3f(this->diffuse, "material.diffuse");
		programId.setVec3f(this->specular, "material.specular");
		programId.set1i(this->diffuseTex, "material.diffuseTex");
		programId.set1i(this->specularTex, "material.specularTex");
	}

	// Getters
	GLint getDiffuseTex() {
		return diffuseTex;
	}

	GLint getSpecTex() {
		return specularTex;
	}

	// Setters
	void changeTexture(GLint newTexture) {
		this->diffuseTex = newTexture;
	}
};
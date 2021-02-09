#pragma once

#include<iostream>
#include<vector>

#include"Primitives.h"
#include"Shader.h"
#include"Texture.h"
#include"Material.h"
#include"Vertex.h"

class Mesh {
private:
	unsigned nVertices, nIndices;
	GLuint VAO, VBO, EBO;
	Texture* diffuseTexture;
	Texture* specTexture;
	Material* material;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 ModelMatrix;


	// Init Vertex Array with given promitive
	void initVAO(Primitive* primitive) {
		this->nVertices = primitive->getNvertices();
		this->nIndices = primitive->getNindices();

		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nVertices * sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW);

		glGenBuffers(1, &this->EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nIndices * sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);

		// POSITION
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		// COLOR
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);


		// TEX COORD
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);

		// NORMAL
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);


		glBindVertexArray(0);
	}


	// Update Model Matrix uniform
	void updateUniforms(Shader* shader) {
		shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
	}

	// Update Model Matrix using changed position, rotation and scale in each frame
	void updateModelMatrix() {
		this->ModelMatrix = glm::mat4(1.0f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	}

public:
	// Constructors
	Mesh(Primitive* primitive, Texture* diffuse, Texture* spec, Material* mat,
		glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f)) {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		this->diffuseTexture = diffuse;
		this->specTexture = spec;
		this->material = mat;

		this->initVAO(primitive);
		this->updateModelMatrix();
	}

	// Destructors
	~Mesh() {
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}


	// Render this shader program
	void render(Shader* shader) {
		// Send Material
		this->material->sendToShader(*shader);
		
		// Update Model Matrices and uniforms
		this->updateModelMatrix();
		this->updateUniforms(shader);

		// Use shader
		shader->use();

		// Bind Textures to material texture units
		this->diffuseTexture->bind(this->material->getDiffuseTex());
		this->specTexture->bind(this->material->getSpecTex());
		
		// Bind Vertex Array
		glBindVertexArray(this->VAO);

		// Draw
		if (this->nIndices == 0) {
			glDrawArrays(GL_TRIANGLES, 0, this->nVertices);
		}
		if (this->nVertices == 2) {
			glDrawElements(GL_LINES, this->nIndices, GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawElements(GL_TRIANGLES, this->nIndices, GL_UNSIGNED_INT, 0);
		}
	}

	// Setters and Modifiers
	void setPosition(const glm::vec3& position) {
		this->position = position;
	}

	void setRotation(const glm::vec3& rotation) {
		this->rotation = rotation;
	}

	void setScale(const glm::vec3& scale) {
		this->scale = scale;
	}

	void moveIt(const glm::vec3& moveVector) {
		this->position += moveVector;
	}

	void rotateIt(const glm::vec3& degree) {
		this->rotation += degree;
	}

	void scaleIt(const glm::vec3& scale) {
		this->scale += scale;
		if (this->scale.x < 0) {
			this->scale.x = 0.f;
		}
		if (this->scale.y < 0) {
			this->scale.y = 0.f;
		}
		if (this->scale.z < 0) {
			this->scale.z = 0.f;
		}
	}

	void changeTexture(Texture* diffuse, Texture* spec) {
		this->diffuseTexture = diffuse;
		this->specTexture = spec;
	}

	// Getters
	unsigned getNindices() {
		return this->nIndices;
	}

	glm::vec3 getPosition() {
		return this->position;
	}
};
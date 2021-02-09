#pragma once

#include<vector>
#include"Vertex.h"
#include<glew.h>
#include<glfw3.h>

class Primitive {
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
public:
	Primitive() {

	}
	virtual ~Primitive() {

	}

	void set(const Vertex* vertecis, const unsigned nVertecis, const GLuint* indices, const unsigned nIndecis) {
		for (size_t i = 0; i < nVertecis; i++)
		{
			this->vertices.push_back(vertecis[i]);
		}
		for (size_t i = 0; i < nIndecis; i++)
		{
			this->indices.push_back(indices[i]);
		}
	}

	inline Vertex* getVertices() {
		return this->vertices.data();
	}

	inline GLuint* getIndices() {
		return this->indices.data();
	}

	inline const unsigned getNvertices() {
		return this->vertices.size();
	}

	inline const unsigned getNindices() {
		return this->indices.size();
	}
};

class Triangle : public Primitive {
public:
	Triangle(glm::vec3 vertex1 = glm::vec3(-0.5f, 0.f, 0.0f),
		glm::vec3 vertex2 = glm::vec3(0.f, 0.5f, 0.0f),
		glm::vec3 vertex3 = glm::vec3(0.5f, 0.f, 0.0f)) : Primitive() {
		Vertex vertices[] =
		{
			vertex1,	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f),	 glm::vec3(0.0f, 0.0f, 1.0f),
			vertex2,	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),	 glm::vec3(0.0f, 0.0f, 1.0f),
			vertex3,	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	 glm::vec3(0.0f, 0.0f, 1.0f),
		};
		unsigned number_of_vertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2
		};
		unsigned number_of_indices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, number_of_vertices, indices, number_of_indices);
	}
};


class Quad : public Primitive {
public:
	Quad() : Primitive() {
		Vertex vertices[] =
		{
			glm::vec3(-0.5f, 0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f),	 glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),	 glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.5f, -0.5f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f),	 glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.5f, 0.5f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 1.0f),	 glm::vec3(0.0f, 0.0f, 1.0f)
		};
		unsigned number_of_vertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2,
			0, 2, 3
		};
		unsigned number_of_indices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, number_of_vertices, indices, number_of_indices);
	}
};

class Pyramid : public Primitive
{
public:
	Pyramid()
		: Primitive()
	{
		Vertex vertices[] =
		{
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),

			glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 1.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),

			glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 1.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),

			glm::vec3(0.f, 0.5f, 0.f),				glm::vec3(1.f, 1.f, 0.f),		glm::vec2(0.5f, 1.f),		glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2,
			3, 4, 5,
			6, 7, 8,
			9, 10, 11,

			1, 11, 2,
			1, 8, 11


		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class Cube : public Primitive
{
public:
	Cube()
		: Primitive()
	{
		Vertex vertices[] =
		{
			glm::vec3(-0.5f, 0.5f, 0.5f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 0.5f, 0.5f),			glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),

			glm::vec3(0.5f, 0.5f, -0.5f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, 0.5f, -0.5f),			glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f)


		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2,
			0, 2, 3,

			7, 6, 1,
			7, 1, 0,

			4, 5, 6,
			4, 6, 7,

			3, 2, 5,
			3, 5, 4,

			4, 0, 3,
			0, 4, 7,

			1, 6, 5,
			1, 5, 2

		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};

class Prism : public Primitive
{
public:
	Prism()
		: Primitive()
	{
		Vertex vertices[] =
		{
			glm::vec3(0.f, 1.0f, 0.5f),				glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 1.f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, 1.f, -0.5f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			
			glm::vec3(0.f, -1.f, 0.5f),				glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -1.f, -0.5f),			glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -1.f, -0.5f),			glm::vec3(0.f, 1.f, 0.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f)

		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1, 2,
			3, 5, 4,
			
			0, 3, 4,
			0, 4, 1,
			
			0, 5, 3,
			0, 2, 5,

			1, 4, 5,
			1, 5, 2
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};


class Line : public Primitive {
public:
	Line(glm::vec3 vertex1, glm::vec3 vertex2) : Primitive() {
		Vertex vertices[] =
		{
			vertex1,	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f),	 glm::vec3(0.0f, 0.0f, 1.0f),
			vertex2,	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),	 glm::vec3(0.0f, 0.0f, 1.0f)
		};
		unsigned number_of_vertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0, 1
		};
		unsigned number_of_indices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, number_of_vertices, indices, number_of_indices);
	}
};
#pragma once

#include <glad/glad.h>

struct EBO {
	GLuint ID;
	EBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};
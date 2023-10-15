#include <glad/glad.h>
#include "renderer.h"
#include "../vertex.h"

#include <stdio.h>

void ErrorCallback(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}

bool libRenderer::Renderer::start()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        return false;
    }

	glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Renderer", NULL, NULL);
	if( window == NULL ){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	int version = gladLoadGL();
	if (version == 0) {
		fprintf(stderr, "Failed to initialize OpenGL context.\n" );
		glfwTerminate();
		return false;
	}

    // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    return true;
}

void libRenderer::Renderer::close()
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void libRenderer::Renderer::run()
{
    do{
		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the meshes.
		for(int i = 0; i < vertexArrays.size(); i++)
		{
			VertexBufferHandle handle = vertexArrays[i];
			glBindVertexArray(handle.vertexArrayObject);
			glDrawElements(GL_TRIANGLES, handle.mesh->trianglesCount() * 3, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void libRenderer::Renderer::addMesh(libMesh::Mesh *mesh)
{
	GLuint vertexArrayObject;
	GLuint vertexBufferObject, elementBufferObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glGenBuffers(1, &vertexBufferObject);
	glGenBuffers(1, &elementBufferObject);

	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, mesh->verticesCount() * sizeof(libMesh::Vertex), mesh->getVertexPtr(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->trianglesCount() * 3 * sizeof(unsigned int), mesh->getIndexPtr(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(libMesh::Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(libMesh::Vertex), (void*)offsetof(libMesh::Vertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(libMesh::Vertex), (void*)offsetof(libMesh::Vertex, texCoord));

	// Reset state machine
	glBindVertexArray(0);
	VertexBufferHandle handle = { vertexArrayObject, mesh };
	vertexArrays.push_back(handle);
}

void libRenderer::Renderer::removeMesh(libMesh::Mesh *mesh)
{
	unsigned int needle = mesh->getId();
	for(int i; i < vertexArrays.size(); i++) {
		if (vertexArrays[0].mesh->getId() == needle)
		{
			vertexArrays.erase(vertexArrays.begin() + i);
		}
	}
}

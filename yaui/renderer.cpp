#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "../vertex.h"

#include <ostream>

void ErrorCallback(int error, const char *description) {
	std::cerr << "Error: " << description << std::endl;
}

const char *vertexShaderSource =
	"#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = 
	"#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";


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
	glfwMakeContextCurrent((GLFWwindow *)window);

	int version = gladLoadGL();
	if (version == 0) {
		fprintf(stderr, "Failed to initialize OpenGL context.\n" );
		glfwTerminate();
		return false;
	}

	// build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Ensure we can capture the escape key being pressed below
	glfwSetInputMode((GLFWwindow *)window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    return true;
}

void libRenderer::Renderer::close()
{
	// Delete OpenGL resources.
	for(int i = 0; i < vertexArrays.size(); i++)
	{
		VertexBufferHandle handle = vertexArrays[i];
		glDeleteVertexArrays(1, &handle.vertexArrayObject);
		glDeleteBuffers(1, &handle.vertexBufferObject);
		glDeleteBuffers(1, &handle.elementBufferObject);
	}
    glDeleteProgram(shaderProgram);

	// Close the window
    glfwSetWindowShouldClose((GLFWwindow *)window, GLFW_TRUE);
}

void libRenderer::Renderer::run()
{
	auto win = (GLFWwindow *)window;
    do{
		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate the shader.
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		float greenValue = 0.5f;
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glUseProgram(shaderProgram);

    	// Draw the meshes.
		for(int i = 0; i < vertexArrays.size(); i++)
		{
			VertexBufferHandle handle = vertexArrays[i];
			glBindVertexArray(handle.vertexArrayObject);
			glDrawArrays(GL_TRIANGLES, 0, handle.mesh->trianglesCount() * 3);
		}
		glBindVertexArray(0);

		// Swap buffers
		glfwSwapBuffers(win);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(win) == 0 );

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
	VertexBufferHandle handle = { vertexArrayObject, vertexBufferObject, elementBufferObject, mesh };
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

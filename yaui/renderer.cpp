#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "../vertex.h"

#include <ostream>
#include <vector>

void ErrorCallback(int error, const char *description) {
	std::cerr << "Error: " << error << ", " << description << std::endl;
}

void ErrorCallback(int error, const char *desc0, const char*desc1) {
	std::cerr << "Error: " << error << ", "  << desc0 << " " << desc1 << std::endl;
}

bool YetAnotherUI::Renderer::start()
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
		ErrorCallback(0, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent((GLFWwindow *)window);

	int version = gladLoadGL();
	if (version == 0) {
		ErrorCallback(0, "Failed to initialize OpenGL context.");
		glfwTerminate();
		return false;
	}

    // Ensure we can capture the escape key being pressed below
	glfwSetInputMode((GLFWwindow *)window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    return true;
}

void YetAnotherUI::Renderer::close()
{
	// Delete OpenGL resources.
	for(int i = 0; i < mRenderables.size(); i++)
	{
		mRenderables[i]->Destroy();
	}
	mRenderables.clear();
    glDeleteProgram(shaderProgram);

	// Close the window
    glfwSetWindowShouldClose((GLFWwindow *)window, GLFW_TRUE);
}

void YetAnotherUI::Renderer::run()
{
	auto win = (GLFWwindow *)window;
    do{
		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT);

    	// Draw the renderables.
		for(int i = 0; i < mRenderables.size(); i++)
		{
			mRenderables[i]->Render();
		}

		// Swap buffers
		glfwSwapBuffers(win);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(win, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(win) == 0 );

    glDeleteProgram(shaderProgram);
    
    // Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void YetAnotherUI::Renderer::add(Renderable *renderable)
{
	renderable->Initialize();
	mRenderables.push_back(renderable);
}

void YetAnotherUI::Renderer::remove(Renderable *renderable)
{
	renderable->Destroy();
	for (auto i = mRenderables.begin(); i < mRenderables.end(); i++) {
        if(*i == renderable) {
			mRenderables.erase(i);
		}
    }
}

#include "renderer.h"

#include <stdio.h>

bool libRenderer::Renderer::start()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        return false;
    }

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

    // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    return true;
}

void libRenderer::Renderer::render()
{
    do{
		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw nothing.

		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}
#include <iostream>
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL\GLU.h>

#include "Shader.h"
#include "Vertex.h"

int main(int argc, char ** argsv)
{

	//Initialises the SDL Library, passing in SDL_INIT_VIDEO to only initialise the video subsystems
	//https://wiki.libsdl.org/SDL_Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//Display an error message box
		//https://wiki.libsdl.org/SDL_ShowSimpleMessageBox
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init failed", SDL_GetError(), NULL);
		return 1;
	}

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
	//Checks to see if the window has been created, the pointer will have a value of some kind
	if (window == nullptr)
	{
		//Show error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow failed", SDL_GetError(), NULL);
		//Close the SDL Library
		//https://wiki.libsdl.org/SDL_Quit
		SDL_Quit();
		return 1;
	}

	

	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Unable to initialise GLEW", (char*)glewGetErrorString(glewError), NULL);
	}

	//Create a Vertex Array object to deal with vertex formats
	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);


	// An array of 3 vectors which represents 3 vertices
	Vertex vertices[] = {
		{-1.0f, -1.0f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f},	// V0
		{1.0f, -1.0f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f},	// V1
		{1.0f, 1.0f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f},		// V2
		{-1.0f,  1.0f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f},	// V3
		{-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},	// V4
		{-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},	// V5
		{0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},	// V6
		{2.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.5f, 1.0f},		// V7
		{-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f},	// V8
		{1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f},	// V9
		{0.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f},	// V10
		{2.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.5f, 1.0f}		// V11

	};


	// This will identify our vertex buffer
	GLuint vertexBuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexBuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);


	// Create buffer function
	unsigned int indicies[] = { 2, 1, 0, 
								3, 0, 2,
								0, 3, 5,
								5, 0, 4 
	};

	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * (sizeof(unsigned int)), indicies, GL_STATIC_DRAW);

	GLuint programID = LoadShaders("BasicVert.glsl", "BasicFrag.glsl");
	GLuint location = glGetUniformLocation(programID, "colorWeights");
	GLuint modelMatrixLocation = glGetUniformLocation(programID, "model");
	GLuint viewMatrixLocation = glGetUniformLocation(programID, "viewMat");
	GLuint projectionMatrixLocation = glGetUniformLocation(programID, "projectionMat");

	srand(static_cast <unsigned> (time(0)));

	// Translate
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	// Translation matrix set to identity
	glm::mat4 translation = glm::mat4(1.0f);
	// Create a translation matrix from given position
	translation = glm::translate(translation, position);
	
	glm::vec3 worldSpaceUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// Camera initialization
	glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 camTarget = position;
	glm::vec3 camDirection = glm::normalize(camPosition - camTarget);
	glm::vec3 camRight = glm::normalize(glm::cross(worldSpaceUp, camDirection));
	glm::vec3 camUp = glm::cross(camDirection, camRight);

	#pragma region Camera Free Rotation
	const float radius = 10.0f;
	
	#pragma endregion

	glm::mat4 view = glm::lookAt(camPosition, camTarget, worldSpaceUp);

	//Perspective Matrix. Every value must be a float
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f/720.0f, 0.1f, 100.0f);


	//Event loop, we will loop until running is set to false, usually if escape has been pressed or window is closed
	bool running = true;
	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;
	while (running)
	{
		//Poll for the events which have happened in this frame
		//https://wiki.libsdl.org/SDL_PollEvent
		while (SDL_PollEvent(&ev))
		{
			//Switch case for every message we are intereted in
			switch (ev.type)
			{
				//QUIT Message, usually called when the window has been closed
			case SDL_QUIT:
				running = false;
				break;
				//KEYDOWN Message, called when a key has been pressed down
			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					//Escape key
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_w:
					camTarget.x -= 1;
					std::cout << camTarget.x << std::endl;
				}
			}
		}

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		float r = static_cast <float> (rand() % 100) / 100.0f;
		float g = static_cast <float> (rand() % 100) / 100.0f;
		float b = static_cast <float> (rand() % 100) / 100.0f;

		// Send constant uniforms to shader
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(translation));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection));


		glUseProgram(programID);

		glUniform3f(location, r, g, b);

		// bind our buffers a second time to be defensive
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

		// Remember to do this (eventually) https://en.cppreference.com/w/cpp/types/offsetof
		// X Y Z
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(Vertex),                  // stride
			(void*)0            // array buffer offset
		);
		// RGBA
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			4,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			(void*)(3 * (sizeof(float)))
		);
		
		// Draw the triangle !
		//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(window);
	}
	glDeleteProgram(programID);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArray);
	SDL_GL_DeleteContext(glContext);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);
	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
}
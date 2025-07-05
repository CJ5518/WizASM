
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include "stdio.h"
#include "glm/glm.hpp"
#include "cj3d/VertexArray.hpp"
#include "cj3d/BufferObject.hpp"
#include "cj3d/Shader.hpp"
#include "cj3d/Camera.hpp"
#include "cj3d/Model.hpp"

int main() {
	SDL_Window *window;
	SDL_GLContext glContext;
	SDL_Event event;

	SDL_SetAppMetadata("Example HUMAN READABLE NAME", "1.0", "com.example.CATEGORY-NAME");
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		printf("Couldn't initialize SDL: %s", SDL_GetError());
		return 1;
	}
	window = SDL_CreateWindow("Le bon titre", 640, 480, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		printf("Couldn't create window/renderer: %s", SDL_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL) {
		printf("Couldn't create GL context, SDL Error: %s\n", SDL_GetError());
	}

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("Error initializing GLEW! %s\n", glewGetErrorString(err));
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//Draws in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	
	cj::Model model;
	
	//model.loadModel();
	model.loadModelFromObj("cube.obj");
	model.modelMatrix = glm::translate(model.modelMatrix, glm::vec3(0.0f,0,1.0f));
	cj::Camera camera;
	camera.sensitivity = 0.1f;

	bool keepRunning = true;
	Uint64 frameStart;
	Uint64 lastStart = SDL_GetTicks() - 1;
	while (keepRunning) {
		frameStart = SDL_GetTicks();
		Uint64 frameDiffMillis = frameStart - lastStart;
		lastStart = frameStart;
		//Check events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				keepRunning = false;
				break;
			}
			camera.processEvent(event);
		}
		camera.fixedUpdate();
		camera.variableUpdate(frameDiffMillis / 1000.0f);

		//Draw stuff
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//model.modelMatrix = glm::rotate(model.modelMatrix, 0.01f, glm::vec3(1.0f,0,0));
		model.shader.setMat4("modelMatrix", glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		model.shader.setMat4("proj", glm::perspective(glm::radians(45.0f), (float)640 / (float)480, 0.1f, 1000.0f));
		model.shader.setMat4("view", camera.viewMatrix);
		model.draw();

		//End of drawing stuff
		SDL_GL_SwapWindow(window);


		if (SDL_GetTicks() - frameStart < 16) {
			SDL_Delay(16 - (SDL_GetTicks() - frameStart));
		}
	}

	model.destroy();

	SDL_DestroyWindow(window);
	SDL_GL_DestroyContext(glContext);  

	SDL_Quit();

	return 0;
}



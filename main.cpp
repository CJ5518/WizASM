
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include "stdio.h"
#include "glm/glm.hpp"
#include "cj3d/VertexArray.hpp"
#include "cj3d/VertexBuffer.hpp"
#include "cj3d/Shader.hpp"
#include "cj3d/Camera.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uvCoords;
};

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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//Set up the triangle
	Vertex triangle[3];
	triangle[0].position = glm::vec3(-0.5f, -0.5f, 0.0f);
	triangle[1].position = glm::vec3(0.5f, -0.5f, 0.0f);
	triangle[2].position = glm::vec3(0.0f, 0.5f, 0.0f);

	cj::VertexArray VAO;
	cj::VertexBuffer<Vertex> VBO;

	VAO.create();
	VAO.bind();

	VBO.create(&triangle, 3, 3 * sizeof(Vertex));
	VBO.bind();

	VAO.setBufferAttribute(sizeof(glm::vec3));
	VAO.setBufferAttribute(sizeof(glm::vec3));
	VAO.setBufferAttribute(sizeof(glm::vec2));
	VAO.flushAttributes();

	glm::mat4 modelMatrix = glm::identity<glm::mat4>();

	cj::Shader shader;
	shader.loadFromFiles("shaders/vertex.glsl", "shaders/frag.glsl");
	shader.use();
	

	cj::Camera camera;

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

		modelMatrix = glm::rotate(modelMatrix, 0.01f, glm::vec3(1.0f,0,0));
		shader.setMat4("modelMatrix", modelMatrix);
		shader.setMat4("pv", glm::perspective(3.14f / 2.0f, 4.0f/3.0f,0.001f, 100.0f));
		SDL_MouseButtonFlags mouseButtons = SDL_GetMouseState(NULL,NULL);
		printf("Mouse: %x\n", mouseButtons);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//End of drawing stuff
		SDL_GL_SwapWindow(window);


		if (SDL_GetTicks() - frameStart < 16) {
			SDL_Delay(16 - (SDL_GetTicks() - frameStart));
		}
	}


	VAO.destroy();
	VBO.destroy();

	SDL_DestroyWindow(window);
	SDL_GL_DestroyContext(glContext);  

	SDL_Quit();

	return 0;
}



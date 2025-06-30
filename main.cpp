
#include <SDL3/SDL.h>
#include <GL/glew.h>
#include "stdio.h"
#include "glm/glm.hpp"
#include "cj3d/VertexArray.hpp"
#include "cj3d/VertexBuffer.hpp"
#include "cj3d/Shader.hpp"

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

	cj::Shader shader;
	shader.loadFromFiles("shaders/vertex.glsl", "shaders/frag.glsl");
	shader.use();

	bool keepRunning = true;
	while (keepRunning) {
		//Check events
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				keepRunning = false;
				break;
			}
		}

		//Draw stuff
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		//End of drawing stuff
		SDL_GL_SwapWindow(window);
	}


	VAO.destroy();
	VBO.destroy();

	SDL_DestroyWindow(window);
	SDL_GL_DestroyContext(glContext);  

	SDL_Quit();

	return 0;
}



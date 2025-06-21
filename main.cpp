
#include <SDL3/SDL.h>
#include "stdio.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int main() {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;

	SDL_SetAppMetadata("Example HUMAN READABLE NAME", "1.0", "com.example.CATEGORY-NAME");
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return 1;
	}

	if (!SDL_CreateWindowAndRenderer("Le bon titre", 640, 480, SDL_WINDOW_OPENGL, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return 1;
	}

	bool keepRunning = true;
	while (keepRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				keepRunning = false;
				break;
			}
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}



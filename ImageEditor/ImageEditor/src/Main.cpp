#include <stdio.h>

#include "SDL.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

int main(int argc, char* args[])
{
	bool running = true;

	SDL_Window* window = nullptr;
	SDL_Surface* screen = nullptr;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL can't be initialized. SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		window = SDL_CreateWindow("Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(window == nullptr)
		{
			printf("Window can't be created. SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			screen = SDL_GetWindowSurface(window);
			if (screen == nullptr)
			{
				printf("Screen can't be created. SDL_Error: %s\n", SDL_GetError());
			}
		}
	}

	while(running)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT)
			{
				running = false;
			}
		}

		SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_UpdateWindowSurface(window);
	}

	SDL_FreeSurface(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
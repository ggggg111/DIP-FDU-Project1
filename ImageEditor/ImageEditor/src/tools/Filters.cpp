#include <iostream>

#include "SDL.h"

#include "modules/Renderer.h"
#include "modules/Window.h"
#include "Application.h"
#include "Filters.h"

void Filters::ApplyGrayScale(SDL_Texture* target, SDL_Texture* filter)
{
	App->renderer->SetRenderTarget(target);

	SDL_Surface* target_surface = SDL_CreateRGBSurface(
		0,
		App->window->width, App->window->height,
		32,
		0, 0, 0, 0
	);

	if (SDL_RenderReadPixels(App->renderer->renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, target_surface->pixels, target_surface->pitch) != 0)
	{
		printf("Error reading pixels. SDL_GetError: %s\n", SDL_GetError());
	}

	Uint32* target_pixels = (Uint32*)target_surface->pixels;
	{
		/*int pitch;
		void* pixels;
		if (SDL_LockTexture(target, nullptr, (void**)&pixels, &pitch))
		{
			printf("Texture can't be locked. SDL_GetError: %s\n", SDL_GetError());
		}

		target_pixels = (Uint32*)pixels;

		SDL_UnlockTexture(target);*/

		if (target_pixels) printf("Not null, good!\n");
	}

	App->renderer->SetRenderTarget(nullptr);
}
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
		SDL_SetTextureBlendMode(filter, SDL_BLENDMODE_BLEND);

		int pitch, w, h;
		void* filter_pixels;

		SDL_QueryTexture(filter, nullptr, nullptr, &w, &h);

		if (SDL_LockTexture(filter, nullptr, (void**)&filter_pixels, &pitch))
		{
			printf("Texture can't be locked. SDL_GetError: %s\n", SDL_GetError());
		}

		Uint32* u_filter_pixels = (Uint32*)filter_pixels;

		Uint32 transparent = SDL_MapRGBA(SDL_GetWindowSurface(App->window->window)->format, 255, 255, 255, 0x00);

		for (int i = 0; i < w * h; i++)
		{
			if (u_filter_pixels[i] == transparent)
				u_filter_pixels[i] = target_pixels[i];
		}

		memcpy(filter_pixels, u_filter_pixels, (pitch / 4) * h);

		SDL_UnlockTexture(target);
	}

	App->renderer->SetRenderTarget(nullptr);
}
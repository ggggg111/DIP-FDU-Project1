#include <iostream>

#include "SDL.h"

#include "modules/Renderer.h"
#include "modules/Window.h"
#include "modules/Editor.h"
#include "Application.h"
#include "Filters.h"

void Filters::ApplyGrayScale(SDL_Texture* target, SDL_Texture* filter)
{
	App->renderer->SetRenderTarget(target);

	Uint32 format = SDL_GetWindowPixelFormat(App->window->window);
	SDL_PixelFormat* pixel_format = SDL_AllocFormat(format);

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

	Uint32* u_target_pixels = (Uint32*)target_surface->pixels;

	App->renderer->SetRenderTarget(nullptr);
		
	//SDL_SetTextureBlendMode(filter, SDL_BLENDMODE_BLEND);

	int pitch, w, h;
	void* filter_pixels;

	SDL_QueryTexture(filter, nullptr, nullptr, &w, &h);

	if (SDL_LockTexture(filter, nullptr, (void**)&filter_pixels, &pitch))
	{
		printf("Texture can't be locked. SDL_GetError: %s\n", SDL_GetError());
	}

	Uint32* u_filter_pixels = (Uint32*)filter_pixels;
	
	for (int i = 0; i < w * h; ++i)
	{
		Uint8 target_r, target_g, target_b;
		SDL_GetRGB(u_target_pixels[i], pixel_format, &target_r, &target_g, &target_b);
		
		Uint8 grayscale = (target_r + target_g + target_b) / 3;
		u_filter_pixels[i] = SDL_MapRGB(pixel_format, grayscale, grayscale, grayscale);
	}

	memcpy(filter_pixels, u_filter_pixels, (pitch / 4) * h);

	SDL_UnlockTexture(filter);

	SDL_FreeFormat(pixel_format);

	App->editor->RenderImg(App->renderer->renderer, filter, target);
}
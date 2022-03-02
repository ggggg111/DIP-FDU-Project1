#include <iostream>

#include "SDL.h"

#include "modules/Renderer.h"
#include "modules/Window.h"
#include "modules/Editor.h"
#include "Application.h"
#include "Filters.h"

void Filters::ApplyGrayScale(SDL_Texture* target, SDL_Texture* filter)
{
	Uint32 format = App->renderer->texture_format;
	SDL_PixelFormat* pixel_format = SDL_AllocFormat(format);

	int target_w;
	SDL_QueryTexture(target, nullptr, nullptr, &target_w, nullptr);

	int target_pitch = target_w * 4; // 4 is bytes per pixel of the texture, given we use SDL_PIXELFORMAT_RGBA8888

	App->renderer->SetRenderTarget(target);

	void* target_pixels = nullptr;
	if (SDL_RenderReadPixels(App->renderer->renderer, nullptr, 0, target_pixels, target_pitch) != 0)
	{
		printf("Error reading pixels. SDL_GetError: %s\n", SDL_GetError());
	}

	App->renderer->SetRenderTarget(nullptr);

	Uint32* u_target_pixels = (Uint32*)target_pixels;
		
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
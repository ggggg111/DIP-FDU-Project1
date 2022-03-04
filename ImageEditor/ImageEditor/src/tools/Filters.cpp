#include <iostream>
#include <vector>

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

	int target_w, target_h;
	SDL_QueryTexture(target, nullptr, nullptr, &target_w, &target_h);

	App->renderer->SetRenderTarget(target);

	SDL_Surface* target_surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		target_w, target_h,
		32,
		App->renderer->texture_format
	);

	if (SDL_RenderReadPixels(
		App->renderer->renderer,
		nullptr,
		App->renderer->texture_format,
		target_surface->pixels,
		target_surface->pitch)
		!= 0
		)
	{
		printf("Error reading pixels. SDL_GetError: %s\n", SDL_GetError());
	}

	App->renderer->SetRenderTarget(nullptr);

	Uint32* u_target_pixels = (Uint32*)target_surface->pixels;
	
	int pitch, w, h;
	SDL_QueryTexture(filter, nullptr, nullptr, &w, &h);

	void* filter_pixels;

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

	SDL_FreeSurface(target_surface);

	SDL_FreeFormat(pixel_format);

	App->editor->RenderImg(App->renderer->renderer, filter, target);
}

void Filters::ApplyBlur(SDL_Texture* target, SDL_Texture* filter, const int& kernel_size)
{
	Uint32 format = App->renderer->texture_format;
	SDL_PixelFormat* pixel_format = SDL_AllocFormat(format);

	int target_w, target_h;
	SDL_QueryTexture(target, nullptr, nullptr, &target_w, &target_h);

	App->renderer->SetRenderTarget(target);

	SDL_Surface* target_surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		target_w, target_h,
		32,
		App->renderer->texture_format
	);

	if (SDL_RenderReadPixels(
		App->renderer->renderer,
		nullptr,
		App->renderer->texture_format,
		target_surface->pixels,
		target_surface->pitch)
		!= 0
		)
	{
		printf("Error reading pixels. SDL_GetError: %s\n", SDL_GetError());
	}

	App->renderer->SetRenderTarget(nullptr);

	Uint32* u_target_pixels = (Uint32*)target_surface->pixels;

	int pitch, w, h;
	SDL_QueryTexture(filter, nullptr, nullptr, &w, &h);

	void* filter_pixels;

	if (SDL_LockTexture(filter, nullptr, (void**)&filter_pixels, &pitch))
	{
		printf("Texture can't be locked. SDL_GetError: %s\n", SDL_GetError());
	}

	Uint32* u_filter_pixels = (Uint32*)filter_pixels;

	std::vector<float> kernel(kernel_size * kernel_size, 0.0f);
	
	for (int i = 0; i < kernel_size; ++i)
	{
		for (int j = 0; j < kernel_size; ++j)
		{
			kernel[i * kernel_size + j] = 1.0f / 9.0f;
		}
	}

	Uint32** u_target_pixels_2d;
	u_target_pixels_2d = new Uint32*[h];
	for (int i = 0; i < h; ++i)
		u_target_pixels_2d[i] = new Uint32[w];

	Uint32** u_filter_pixels_2d;
	u_filter_pixels_2d = new Uint32 * [h];
	for (int i = 0; i < h; ++i)
		u_filter_pixels_2d[i] = new Uint32[w];

	for (int row = 0; row < h; ++row)
	{
		for (int col = 0; col < w; ++col)
		{
			u_target_pixels_2d[row][col] = u_target_pixels[(row * w + col)];
		}
	}

	for (int row = 0; row < h; ++row)
	{
		for (int col = 0; col < w; ++col)
		{
			Uint8 filter_r, filter_g, filter_b;

			filter_r = 0;
			filter_g = 0;
			filter_b = 0;

			int krad = kernel_size / 2;
			int k_ind = 0;

			float sum_r = 0.0f;
			float sum_g = 0.0f;
			float sum_b = 0.0f;

			for (int k_row = -krad; k_row <= krad; ++k_row)
			{
				for (int k_col = -krad; k_col <= krad; ++k_col)
				{
					Uint8 target_r = 0, target_g = 0, target_b = 0;
					int target_row = row + k_row;
					int target_col = col + k_col;
					
					if (target_row >= 0 && target_col >= 0
						&& target_row < h && target_col < w)
					{
						SDL_GetRGB(u_target_pixels_2d[target_row][target_col], pixel_format, &target_r, &target_g, &target_b);
					}

					float target_r_normalized = (float)target_r / 255.0f;
					float target_g_normalized = (float)target_g / 255.0f;
					float target_b_normalized = (float)target_b / 255.0f;

					sum_r += kernel[k_ind] * target_r_normalized;
					sum_g += kernel[k_ind] * target_g_normalized;
					sum_b += kernel[k_ind] * target_b_normalized;

					++k_ind;
				}
			}

			filter_r = Uint8(sum_r * 255.0f);
			filter_g = Uint8(sum_g * 255.0f);
			filter_b = Uint8(sum_b * 255.0f);

			u_filter_pixels_2d[row][col] = SDL_MapRGB(pixel_format, filter_r, filter_g, filter_b);
		}
	}

	for (int row = 0; row < h; ++row)
	{
		for (int col = 0; col < w; ++col)
		{
			u_filter_pixels[row * w + col] = u_filter_pixels_2d[row][col];
		}
	}

	memcpy(filter_pixels, u_filter_pixels, (pitch / 4) * h);

	SDL_UnlockTexture(filter);

	for (int i = 0; i < h; ++i)
	{
		delete[] u_target_pixels_2d[i];
	}

	delete[] u_target_pixels_2d;

	for (int i = 0; i < h; ++i)
	{
		delete[] u_filter_pixels_2d[i];
	}

	delete[] u_filter_pixels_2d;

	SDL_FreeSurface(target_surface);

	SDL_FreeFormat(pixel_format);

	App->editor->RenderImg(App->renderer->renderer, filter, target);
}

void Filters::ApplyNegative(SDL_Texture* target, SDL_Texture* filter)
{
	Uint32 format = App->renderer->texture_format;
	SDL_PixelFormat* pixel_format = SDL_AllocFormat(format);

	int target_w, target_h;
	SDL_QueryTexture(target, nullptr, nullptr, &target_w, &target_h);

	App->renderer->SetRenderTarget(target);

	SDL_Surface* target_surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		target_w, target_h,
		32,
		App->renderer->texture_format
	);

	if (SDL_RenderReadPixels(
		App->renderer->renderer,
		nullptr,
		App->renderer->texture_format,
		target_surface->pixels,
		target_surface->pitch)
		!= 0
		)
	{
		printf("Error reading pixels. SDL_GetError: %s\n", SDL_GetError());
	}

	App->renderer->SetRenderTarget(nullptr);

	Uint32* u_target_pixels = (Uint32*)target_surface->pixels;

	int pitch, w, h;
	SDL_QueryTexture(filter, nullptr, nullptr, &w, &h);

	void* filter_pixels;

	if (SDL_LockTexture(filter, nullptr, (void**)&filter_pixels, &pitch))
	{
		printf("Texture can't be locked. SDL_GetError: %s\n", SDL_GetError());
	}

	Uint32* u_filter_pixels = (Uint32*)filter_pixels;

	for (int i = 0; i < w * h; ++i)
	{
		Uint8 target_r, target_g, target_b;
		SDL_GetRGB(u_target_pixels[i], pixel_format, &target_r, &target_g, &target_b);

		Uint8 filter_r = 255 - target_r;
		Uint8 filter_g = 255 - target_g;
		Uint8 filter_b = 255 - target_b;

		u_filter_pixels[i] = SDL_MapRGB(pixel_format, filter_r, filter_g, filter_b);
	}

	memcpy(filter_pixels, u_filter_pixels, (pitch / 4) * h);

	SDL_UnlockTexture(filter);

	SDL_FreeSurface(target_surface);

	SDL_FreeFormat(pixel_format);

	App->editor->RenderImg(App->renderer->renderer, filter, target);
}
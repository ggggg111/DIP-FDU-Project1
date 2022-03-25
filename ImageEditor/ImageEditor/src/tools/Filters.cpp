#include <numeric>

#include "SDL.h"

#include "modules/Renderer.h"
#include "modules/Window.h"
#include "modules/Editor.h"
#include "utils/Utils.h"
#include "Application.h"
#include "Filters.h"

void Filters::ApplyGrayScale(SDL_Texture* target, SDL_Texture* filter)
{
	Uint32 format = App->renderer->texture_format;
	SDL_PixelFormat* pixel_format = SDL_AllocFormat(format);

	int width, height;
	SDL_QueryTexture(target, nullptr, nullptr, &width, &height);

	App->renderer->SetRenderTarget(target);

	SDL_Surface* target_surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		width, height,
		32,
		App->renderer->texture_format
	);

	SDL_RenderReadPixels(
		App->renderer->renderer,
		nullptr,
		App->renderer->texture_format,
		target_surface->pixels,
		target_surface->pitch
	);

	App->renderer->SetRenderTarget(nullptr);

	Uint32* u_target_pixels = (Uint32*)target_surface->pixels;
	
	int pitch;
	void* filter_pixels;

	SDL_LockTexture(filter, nullptr, (void**)&filter_pixels, &pitch);

	Uint32* u_filter_pixels = (Uint32*)filter_pixels;

	for (int i = 0; i < width * height; ++i)
	{
		Uint8 target_r, target_g, target_b;
		SDL_GetRGB(u_target_pixels[i], pixel_format, &target_r, &target_g, &target_b);
		
		Uint8 grayscale = (target_r + target_g + target_b) / 3;

		u_filter_pixels[i] = SDL_MapRGB(pixel_format, grayscale, grayscale, grayscale);
	}

	memcpy(filter_pixels, u_filter_pixels, (pitch / 4) * height);

	SDL_UnlockTexture(filter);
	SDL_FreeSurface(target_surface);
	SDL_FreeFormat(pixel_format);

	App->editor->RenderImg(filter, target, false);
}

void Filters::ApplyBlur(SDL_Texture* target, SDL_Texture* filter, const int& kernel_size)
{
	Uint32 format = App->renderer->texture_format;
	SDL_PixelFormat* pixel_format = SDL_AllocFormat(format);

	int width, height;
	SDL_QueryTexture(target, nullptr, nullptr, &width, &height);

	App->renderer->SetRenderTarget(target);

	SDL_Surface* target_surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		width, height,
		32,
		App->renderer->texture_format
	);

	SDL_RenderReadPixels(
		App->renderer->renderer,
		nullptr,
		App->renderer->texture_format,
		target_surface->pixels,
		target_surface->pitch
	);

	App->renderer->SetRenderTarget(nullptr);

	Uint32* u_target_pixels = (Uint32*)target_surface->pixels;

	int pitch;
	void* filter_pixels;

	SDL_LockTexture(filter, nullptr, (void**)&filter_pixels, &pitch);

	Uint32* u_filter_pixels = (Uint32*)filter_pixels;

	std::vector<float> kernel = Filters::CreateStaticKernel(3, 1.0f / 9.0f);

	Uint32** u_target_pixels_2d = Array2D<Uint32>(width, height);
	Uint32** u_filter_pixels_2d = Array2D<Uint32>(width, height);

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			u_target_pixels_2d[row][col] = u_target_pixels[(row * width + col)];
		}
	}

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
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
						&& target_row < height && target_col < width)
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

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			u_filter_pixels[row * width + col] = u_filter_pixels_2d[row][col];
		}
	}

	memcpy(filter_pixels, u_filter_pixels, (pitch / 4) * height);

	SDL_UnlockTexture(filter);

	RELEASE_ARRAY2D(u_target_pixels_2d, height);
	RELEASE_ARRAY2D(u_filter_pixels_2d, height);

	SDL_FreeSurface(target_surface);

	SDL_FreeFormat(pixel_format);

	App->editor->RenderImg(filter, target, false);
}

void Filters::ApplyGaussianBlur(SDL_Texture* target, SDL_Texture* filter, const int& kernel_size)
{
	Uint32 format = App->renderer->texture_format;
	SDL_PixelFormat* pixel_format = SDL_AllocFormat(format);

	int width, height;
	SDL_QueryTexture(target, nullptr, nullptr, &width, &height);

	App->renderer->SetRenderTarget(target);

	SDL_Surface* target_surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		width, height,
		32,
		App->renderer->texture_format
	);

	SDL_RenderReadPixels(
		App->renderer->renderer,
		nullptr,
		App->renderer->texture_format,
		target_surface->pixels,
		target_surface->pitch
	);

	App->renderer->SetRenderTarget(nullptr);

	Uint32* u_target_pixels = (Uint32*)target_surface->pixels;

	int pitch;
	void* filter_pixels;

	SDL_LockTexture(filter, nullptr, (void**)&filter_pixels, &pitch);

	Uint32* u_filter_pixels = (Uint32*)filter_pixels;

	std::vector<float> kernel = Filters::CreateGaussianKernel(kernel_size);

	Uint32** u_target_pixels_2d = Array2D<Uint32>(width, height);
	Uint32** u_filter_pixels_2d = Array2D<Uint32>(width, height);

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			u_target_pixels_2d[row][col] = u_target_pixels[(row * width + col)];
		}
	}

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
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
						&& target_row < height && target_col < width)
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

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			u_filter_pixels[row * width + col] = u_filter_pixels_2d[row][col];
		}
	}

	memcpy(filter_pixels, u_filter_pixels, (pitch / 4) * height);

	SDL_UnlockTexture(filter);

	RELEASE_ARRAY2D(u_target_pixels_2d, height);
	RELEASE_ARRAY2D(u_filter_pixels_2d, height);

	SDL_FreeSurface(target_surface);

	SDL_FreeFormat(pixel_format);

	App->editor->RenderImg(filter, target, false);
}

void Filters::ApplyMedianBlur(SDL_Texture* target, SDL_Texture* filter, const int& kernel_size)
{
	Uint32 format = App->renderer->texture_format;
	SDL_PixelFormat* pixel_format = SDL_AllocFormat(format);

	int width, height;
	SDL_QueryTexture(target, nullptr, nullptr, &width, &height);

	App->renderer->SetRenderTarget(target);

	SDL_Surface* target_surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		width, height,
		32,
		App->renderer->texture_format
	);

	SDL_RenderReadPixels(
		App->renderer->renderer,
		nullptr,
		App->renderer->texture_format,
		target_surface->pixels,
		target_surface->pitch
	);

	App->renderer->SetRenderTarget(nullptr);

	Uint32* u_target_pixels = (Uint32*)target_surface->pixels;

	int pitch;
	void* filter_pixels;

	SDL_LockTexture(filter, nullptr, (void**)&filter_pixels, &pitch);

	Uint32* u_filter_pixels = (Uint32*)filter_pixels;

	Uint32** u_target_pixels_2d = Array2D<Uint32>(width, height);
	Uint32** u_filter_pixels_2d = Array2D<Uint32>(width, height);

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			u_target_pixels_2d[row][col] = u_target_pixels[(row * width + col)];
		}
	}

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			Uint8 filter_r, filter_g, filter_b;

			filter_r = 0;
			filter_g = 0;
			filter_b = 0;

			int krad = kernel_size / 2;
			int k_ind = 0;

			std::vector<int> r_list(kernel_size * kernel_size);
			std::vector<int> g_list(kernel_size * kernel_size);
			std::vector<int> b_list(kernel_size * kernel_size);

			for (int k_row = -krad; k_row <= krad; ++k_row)
			{
				for (int k_col = -krad; k_col <= krad; ++k_col)
				{
					Uint8 target_r = 0, target_g = 0, target_b = 0;
					int target_row = row + k_row;
					int target_col = col + k_col;

					if (target_row >= 0 && target_col >= 0
						&& target_row < height && target_col < width)
					{
						SDL_GetRGB(u_target_pixels_2d[target_row][target_col], pixel_format, &target_r, &target_g, &target_b);
					}

					r_list[k_ind] = target_r;
					g_list[k_ind] = target_g;
					b_list[k_ind] = target_b;

					++k_ind;
				}
			}

			filter_r = (Uint8)Median(r_list);
			filter_g = (Uint8)Median(g_list);
			filter_b = (Uint8)Median(b_list);

			u_filter_pixels_2d[row][col] = SDL_MapRGB(pixel_format, filter_r, filter_g, filter_b);
		}
	}

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			u_filter_pixels[row * width + col] = u_filter_pixels_2d[row][col];
		}
	}

	memcpy(filter_pixels, u_filter_pixels, (pitch / 4) * height);

	SDL_UnlockTexture(filter);

	RELEASE_ARRAY2D(u_target_pixels_2d, height);
	RELEASE_ARRAY2D(u_filter_pixels_2d, height);

	SDL_FreeSurface(target_surface);

	SDL_FreeFormat(pixel_format);

	App->editor->RenderImg(filter, target, false);
}

void Filters::ApplyLaplace(SDL_Texture* target, SDL_Texture* filter)
{
	Uint32 format = App->renderer->texture_format;
	SDL_PixelFormat* pixel_format = SDL_AllocFormat(format);

	int width, height;
	SDL_QueryTexture(target, nullptr, nullptr, &width, &height);

	App->renderer->SetRenderTarget(target);

	SDL_Surface* target_surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		width, height,
		32,
		App->renderer->texture_format
	);

	SDL_RenderReadPixels(
		App->renderer->renderer,
		nullptr,
		App->renderer->texture_format,
		target_surface->pixels,
		target_surface->pitch
	);

	App->renderer->SetRenderTarget(nullptr);

	Uint32* u_target_pixels = (Uint32*)target_surface->pixels;

	int pitch;
	void* filter_pixels;

	SDL_LockTexture(filter, nullptr, (void**)&filter_pixels, &pitch);

	Uint32* u_filter_pixels = (Uint32*)filter_pixels;

	std::vector<int> kernel = Filters::CreateLaplaceKernel();
	int kernel_size = 3;

	Uint32** u_target_pixels_2d = Array2D<Uint32>(width, height);
	Uint32** u_filter_pixels_2d = Array2D<Uint32>(width, height);

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			u_target_pixels_2d[row][col] = u_target_pixels[(row * width + col)];
		}
	}

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			int krad = kernel_size / 2;
			int k_ind = 0;

			int sum_r = 0;
			int sum_g = 0;
			int sum_b = 0;

			for (int k_row = -krad; k_row <= krad; ++k_row)
			{
				for (int k_col = -krad; k_col <= krad; ++k_col)
				{
					Uint8 k_r = 0, k_g = 0, k_b = 0;
					int target_row = row + k_row;
					int target_col = col + k_col;

					if (target_row >= 0 && target_col >= 0
						&& target_row < height && target_col < width)
					{
						SDL_GetRGB(u_target_pixels_2d[target_row][target_col], pixel_format, &k_r, &k_g, &k_b);
					}

					sum_r += kernel[k_ind] * (int)k_r;
					sum_g += kernel[k_ind] * (int)k_g;
					sum_b += kernel[k_ind] * (int)k_b;

					++k_ind;
				}
			}

			CLAMP(sum_r, 0, 255);
			CLAMP(sum_g, 0, 255);
			CLAMP(sum_b, 0, 255);

			Uint8 current_target_r = 0;
			Uint8 current_target_g = 0;
			Uint8 current_target_b = 0;
			SDL_GetRGB(u_target_pixels_2d[row][col], pixel_format, &current_target_r, &current_target_g, &current_target_b);

			u_filter_pixels_2d[row][col] = SDL_MapRGB(pixel_format, sum_r, sum_g, sum_b);
		}
	}

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			u_filter_pixels[row * width + col] = u_filter_pixels_2d[row][col];
		}
	}

	memcpy(filter_pixels, u_filter_pixels, (pitch / 4) * height);

	SDL_UnlockTexture(filter);

	RELEASE_ARRAY2D(u_target_pixels_2d, height);
	RELEASE_ARRAY2D(u_filter_pixels_2d, height);

	SDL_FreeSurface(target_surface);

	SDL_FreeFormat(pixel_format);

	App->editor->RenderImg(filter, target, false);;
}

void Filters::ApplyLaplaceEnhancement(SDL_Texture* target, SDL_Texture* filter)
{
	Uint32 format = App->renderer->texture_format;
	SDL_PixelFormat* pixel_format = SDL_AllocFormat(format);

	int width, height;
	SDL_QueryTexture(target, nullptr, nullptr, &width, &height);

	App->renderer->SetRenderTarget(target);

	SDL_Surface* target_surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		width, height,
		32,
		App->renderer->texture_format
	);

	SDL_RenderReadPixels(
		App->renderer->renderer,
		nullptr,
		App->renderer->texture_format,
		target_surface->pixels,
		target_surface->pitch
	);

	App->renderer->SetRenderTarget(nullptr);

	Uint32* u_target_pixels = (Uint32*)target_surface->pixels;

	int pitch;
	void* filter_pixels;

	SDL_LockTexture(filter, nullptr, (void**)&filter_pixels, &pitch);

	Uint32* u_filter_pixels = (Uint32*)filter_pixels;

	std::vector<int> kernel = Filters::CreateLaplaceKernel();
	int kernel_size = 3;

	Uint32** u_target_pixels_2d = Array2D<Uint32>(width, height);
	Uint32** u_filter_pixels_2d = Array2D<Uint32>(width, height);

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			u_target_pixels_2d[row][col] = u_target_pixels[(row * width + col)];
		}
	}

	Uint8** initial_target_r = Array2D<Uint8>(width, height);
	Uint8** initial_target_g = Array2D<Uint8>(width, height);
	Uint8** initial_target_b = Array2D<Uint8>(width, height);

	int min_r = 0;
	int min_g = 0;
	int min_b = 0;

	int** sums_r = Array2D<int>(width, height);
	int** sums_g = Array2D<int>(width, height);
	int** sums_b = Array2D<int>(width, height);

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			SDL_GetRGB(u_target_pixels_2d[row][col], pixel_format, &initial_target_r[row][col], &initial_target_g[row][col], &initial_target_b[row][col]);

			int krad = kernel_size / 2;
			int k_ind = 0;

			int sum_r = 0;
			int sum_g = 0;
			int sum_b = 0;

			for (int k_row = -krad; k_row <= krad; ++k_row)
			{
				for (int k_col = -krad; k_col <= krad; ++k_col)
				{
					Uint8 k_r = 0, k_g = 0, k_b = 0;
					int target_row = row + k_row;
					int target_col = col + k_col;

					if (target_row >= 0 && target_col >= 0
						&& target_row < height && target_col < width)
					{
						SDL_GetRGB(u_target_pixels_2d[target_row][target_col], pixel_format, &k_r, &k_g, &k_b);
					}

					sum_r += kernel[k_ind] * (int)k_r;
					sum_g += kernel[k_ind] * (int)k_g;
					sum_b += kernel[k_ind] * (int)k_b;

					++k_ind;
				}
			}

			sums_r[row][col] = sum_r;
			sums_g[row][col] = sum_g;
			sums_b[row][col] = sum_b;

			if (min_r > sum_r) min_r = sum_r;
			if (min_g > sum_g) min_g = sum_g;
			if (min_b > sum_b) min_b = sum_b;
		}
	}

	int** sums_r_min = Array2D<int>(width, height);
	int** sums_g_min = Array2D<int>(width, height);
	int** sums_b_min = Array2D<int>(width, height);

	int max_r = 255;
	int max_g = 255;
	int max_b = 255;

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			sums_r_min[row][col] = sums_r[row][col] - min_r;
			sums_g_min[row][col] = sums_g[row][col] - min_g;
			sums_b_min[row][col] = sums_b[row][col] - min_b;

			if (max_r < sums_r_min[row][col]) max_r = sums_r_min[row][col];
			if (max_g < sums_g_min[row][col]) max_g = sums_g_min[row][col];
			if (max_b < sums_b_min[row][col]) max_b = sums_b_min[row][col];
		}
	}

	int** initial_sharpened_r = Array2D<int>(width, height);
	int** initial_sharpened_g = Array2D<int>(width, height);
	int** initial_sharpened_b = Array2D<int>(width, height);

	int min_sharpened_r = 0;
	int min_sharpened_g = 0;
	int min_sharpened_b = 0;

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			int r = sums_r_min[row][col] * (255.0f / max_r);
			int g = sums_g_min[row][col] * (255.0f / max_g);
			int b = sums_b_min[row][col] * (255.0f / max_b);

			Uint8 wh = (r + g + b) / 3;

			initial_sharpened_r[row][col] = (int)initial_target_r[row][col] + r / (kernel_size * kernel_size);
			initial_sharpened_g[row][col] = (int)initial_target_g[row][col] + g / (kernel_size * kernel_size);
			initial_sharpened_b[row][col] = (int)initial_target_b[row][col] + b / (kernel_size * kernel_size);

			if (min_sharpened_r > initial_sharpened_r[row][col]) min_sharpened_r = initial_sharpened_r[row][col];
			if (min_sharpened_g > initial_sharpened_g[row][col]) min_sharpened_g = initial_sharpened_g[row][col];
			if (min_sharpened_b > initial_sharpened_b[row][col]) min_sharpened_b = initial_sharpened_b[row][col];
		}
	}

	int** final_sharpened_r = Array2D<int>(width, height);
	int** final_sharpened_g = Array2D<int>(width, height);
	int** final_sharpened_b = Array2D<int>(width, height);

	int max_sharpened_r = 255;
	int max_sharpened_g = 255;
	int max_sharpened_b = 255;

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			final_sharpened_r[row][col] = initial_sharpened_r[row][col] - min_sharpened_r;
			final_sharpened_g[row][col] = initial_sharpened_g[row][col] - min_sharpened_g;
			final_sharpened_b[row][col] = initial_sharpened_b[row][col] - min_sharpened_b;

			if (max_sharpened_r < final_sharpened_r[row][col]) max_sharpened_r = final_sharpened_r[row][col];
			if (max_sharpened_g < final_sharpened_g[row][col]) max_sharpened_g = final_sharpened_g[row][col];
			if (max_sharpened_b < final_sharpened_b[row][col]) max_sharpened_b = final_sharpened_b[row][col];
		}
	}

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			int r = final_sharpened_r[row][col] * (255.0f / max_sharpened_r);
			int g = final_sharpened_g[row][col] * (255.0f / max_sharpened_g);
			int b = final_sharpened_b[row][col] * (255.0f / max_sharpened_b);

			u_filter_pixels_2d[row][col] = SDL_MapRGB(
				pixel_format,
				(Uint8)r,
				(Uint8)g,
				(Uint8)b
			);
		}
	}

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			u_filter_pixels[row * width + col] = u_filter_pixels_2d[row][col];
		}
	}

	memcpy(filter_pixels, u_filter_pixels, (pitch / 4) * height);

	SDL_UnlockTexture(filter);

	RELEASE_ARRAY2D(u_target_pixels_2d, height);
	RELEASE_ARRAY2D(u_filter_pixels_2d, height);

	SDL_FreeSurface(target_surface);

	SDL_FreeFormat(pixel_format);

	App->editor->RenderImg(filter, target, false);
}

void Filters::ApplyNegative(SDL_Texture* target, SDL_Texture* filter)
{
	Uint32 format = App->renderer->texture_format;
	SDL_PixelFormat* pixel_format = SDL_AllocFormat(format);

	int width, height;
	SDL_QueryTexture(target, nullptr, nullptr, &width, &height);

	App->renderer->SetRenderTarget(target);

	SDL_Surface* target_surface = SDL_CreateRGBSurfaceWithFormat(
		0,
		width, height,
		32,
		App->renderer->texture_format
	);

	SDL_RenderReadPixels(
		App->renderer->renderer,
		nullptr,
		App->renderer->texture_format,
		target_surface->pixels,
		target_surface->pitch
	);

	App->renderer->SetRenderTarget(nullptr);

	Uint32* u_target_pixels = (Uint32*)target_surface->pixels;

	int pitch;
	void* filter_pixels;

	SDL_LockTexture(filter, nullptr, (void**)&filter_pixels, &pitch);

	Uint32* u_filter_pixels = (Uint32*)filter_pixels;

	for (int i = 0; i < width * height; ++i)
	{
		Uint8 target_r, target_g, target_b;
		SDL_GetRGB(u_target_pixels[i], pixel_format, &target_r, &target_g, &target_b);

		Uint8 filter_r = 255 - target_r;
		Uint8 filter_g = 255 - target_g;
		Uint8 filter_b = 255 - target_b;

		u_filter_pixels[i] = SDL_MapRGB(pixel_format, filter_r, filter_g, filter_b);
	}

	memcpy(filter_pixels, u_filter_pixels, (pitch / 4) * height);

	SDL_UnlockTexture(filter);

	SDL_FreeSurface(target_surface);

	SDL_FreeFormat(pixel_format);

	App->editor->RenderImg(filter, target, false);
}

std::vector<float> Filters::CreateStaticKernel(const int& kernel_size, const float& value)
{
	std::vector<float> kernel(kernel_size * kernel_size, 0.0f);

	for (int i = 0; i < kernel_size; ++i)
	{
		for (int j = 0; j < kernel_size; ++j)
		{
			kernel[i * kernel_size + j] = value;
		}
	}

	return kernel;
}

std::vector<float> Filters::CreateGaussianKernel(const int& kernel_size)
{
	std::vector<float> kernel(kernel_size * kernel_size, 0.0f);

	int krad = kernel_size / 2;
	float sigma = krad / 2.0f;
	float sum = 0;

	auto gaussian = [&](const float& x, const float& mu, const float& sigma)
	{
		float a = (x - mu) / sigma;
		
		return std::exp(-0.5f * a * a);
	};

	for (int i = 0; i < kernel_size; ++i)
	{
		for (int j = 0; j < kernel_size; ++j)
		{
			float x = gaussian(i, krad, sigma) * gaussian(j, krad, sigma);
			
			kernel[i * kernel_size + j] = x;
			
			sum += x;
		}
	}

	for (int i = 0; i < kernel_size; ++i)
	{
		for (int j = 0; j < kernel_size; ++j)
		{
			kernel[i * kernel_size + j] /= sum;
		}
	}

	return kernel;
}

std::vector<int> Filters::CreateLaplaceKernel()
{
	return std::vector<int> { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
}

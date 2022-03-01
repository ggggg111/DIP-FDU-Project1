#include <iostream>

#include "SDL.h"

#include "modules/Renderer.h"
#include "Application.h"
#include "Filters.h"

void Filters::ApplyGrayScale(SDL_Texture* texture)
{
	App->renderer->SetRenderTarget(texture);

	int* pixels = nullptr;
	int pitch;

	if (SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch))
	{
		printf("Texture can't be locked. SDL_GetError: %s\n", SDL_GetError());
	}

	std::cout << pixels << std::endl;
	std::cout << pitch << std::endl;

	//SDL_QueryTexture(texture, nullptr, )

	SDL_UnlockTexture(texture);

	App->renderer->SetRenderTarget(nullptr);
}
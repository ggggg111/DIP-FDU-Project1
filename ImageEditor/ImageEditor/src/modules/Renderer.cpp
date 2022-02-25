#include <iostream>

#include "SDL.h"

#include "Application.h"
#include "Renderer.h"
#include "Window.h"

Renderer::Renderer()
	: renderer(nullptr), texture_target(nullptr)
{

}

Renderer::~Renderer()
{

}

void Renderer::Start()
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	this->renderer = SDL_CreateRenderer(App->window->window, -1, SDL_RENDERER_TARGETTEXTURE);
	if (this->renderer == nullptr)
	{
		printf("Renderer can't be created. SDL_GetError: %s\n", SDL_GetError());
	}

	this->texture_target = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, App->window->width, App->window->height);
}

void Renderer::PreUpdate()
{
	SDL_SetRenderTarget(this->renderer, this->texture_target);
	SDL_SetRenderDrawColor(App->renderer->renderer, 255, 255, 255, 255);
	SDL_RenderClear(App->renderer->renderer);
}

void Renderer::Update()
{
	
}

void Renderer::PostUpdate()
{
	SDL_RenderPresent(App->renderer->renderer);
	SDL_SetRenderTarget(this->renderer, nullptr);

	SDL_RenderClear(this->renderer);
	SDL_RenderCopyEx(this->renderer, this->texture_target, nullptr, nullptr, 0, nullptr, SDL_FLIP_VERTICAL);
	SDL_RenderPresent(this->renderer);
}

void Renderer::CleanUp()
{
	SDL_DestroyRenderer(this->renderer);
}
#include <iostream>

#include "SDL.h"

#include "Application.h"
#include "Renderer.h"
#include "Window.h"

Renderer::Renderer()
	: renderer(nullptr)
{

}

Renderer::~Renderer()
{

}

void Renderer::Start()
{
	this->renderer = SDL_CreateRenderer(App->window->window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (this->renderer == nullptr)
	{
		printf("Renderer can't be created. SDL_GetError: %s\n", SDL_GetError());
	}
}

void Renderer::PreUpdate()
{
	SDL_SetRenderDrawColor(App->renderer->renderer, 255, 255, 255, 255);
	SDL_RenderClear(App->renderer->renderer);
}

void Renderer::Update()
{
	
}

void Renderer::PostUpdate()
{
	SDL_RenderPresent(App->renderer->renderer);
}

void Renderer::CleanUp()
{
	SDL_DestroyRenderer(this->renderer);
}
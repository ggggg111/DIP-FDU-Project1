#include <iostream>

#include "SDL.h"

#include "Application.h"
#include "Renderer.h"
#include "Window.h"
#include "Editor.h"
#include "Gui.h"

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

	this->texture_target = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, App->window->width, App->window->height);
}

void Renderer::PreUpdate()
{

}

void Renderer::Update()
{

}

void Renderer::PostUpdate()
{
	App->gui->PreUpdate();
	App->gui->Update();
	App->gui->PostUpdate();

	SDL_RenderPresent(this->renderer);
}

void Renderer::CleanUp()
{
	SDL_DestroyTexture(texture_target);
	SDL_DestroyRenderer(this->renderer);
}
#include <iostream>
#include <cmath>

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
	SDL_SetRenderTarget(App->renderer->renderer, this->texture_target);
}

void Renderer::Update()
{

}

void Renderer::PostUpdate()
{
	SDL_RenderCopy(App->renderer->renderer, this->texture_target, nullptr, nullptr);

	SDL_SetRenderTarget(App->renderer->renderer, nullptr);

	App->gui->PreUpdate();
	App->gui->Update();
	App->gui->PostUpdate();

	SDL_RenderPresent(this->renderer);
}

void Renderer::CleanUp()
{
	SDL_DestroyTexture(this->texture_target);
	SDL_DestroyRenderer(this->renderer);
}

void Renderer::DrawCircle(const int& x, const int& y, const int& radius)
{
	SDL_Point points[360];
	float factor = (float)M_PI / 180.0f;

	for (int i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	SDL_RenderDrawPoints(this->renderer, points, 360);
}

void Renderer::DrawCircleFill(const int& x, const int& y, const int& radius)
{
	for (int w = 0; w < radius * 2; ++w)
	{
		for (int h = 0; h < radius * 2; ++h)
		{
			int dx = radius - w;
			int dy = radius - h;

			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(this->renderer, x + dx, y + dy);
			}
		}
	}
}
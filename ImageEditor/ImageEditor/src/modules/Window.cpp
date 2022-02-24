#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

#include "Window.h"

Window::Window(const std::string& name, const int& width, const int& height)
	: window(nullptr), name(name), width(width), height(height)
{

}

Window::~Window()
{

}

void Window::Start()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL can't be initialized. SDL_Error: %s\n", SDL_GetError());
	}

	int image_format_flags = IMG_INIT_JPG | IMG_INIT_PNG;

	if ((IMG_Init(image_format_flags) & image_format_flags) != image_format_flags)
	{
		printf("SDL_image can't be initialized. IMG_GetError: %s\n", IMG_GetError());
	}

	this->window = SDL_CreateWindow(this->name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, SDL_WINDOW_SHOWN);
	if (this->window == nullptr)
	{
		printf("Window can't be created. SDL_Error: %s\n", SDL_GetError());
	}

	this->surface = SDL_GetWindowSurface(this->window);
	if (this->surface == nullptr)
	{
		printf("Surface can't be created. SDL_Error: %s\n", SDL_GetError());
	}
}

void Window::CleanUp()
{
	SDL_FreeSurface(this->surface);
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}
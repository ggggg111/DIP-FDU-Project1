#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

#include "Window.h"

Window::Window(const std::string& name, const int& width, const int& height)
	: Module(), window(nullptr), name(name), width(width), height(height)
{

}

Window::~Window()
{

}

void Window::Start()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL can't be initialized. SDL_GetError: %s\n", SDL_GetError());
	}

	int image_format_flags = IMG_INIT_JPG | IMG_INIT_PNG;

	if ((IMG_Init(image_format_flags) & image_format_flags) != image_format_flags)
	{
		printf("SDL_image can't be initialized. IMG_GetError: %s\n", IMG_GetError());
	}

	this->window = SDL_CreateWindow(this->name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width, this->height, SDL_WINDOW_SHOWN);
	if (this->window == nullptr)
	{
		printf("Window can't be created. SDL_GetError: %s\n", SDL_GetError());
	}
}

void Window::CleanUp()
{
	SDL_DestroyWindow(this->window);
	IMG_Quit();
	SDL_Quit();
}

void Window::SetWindowSize(const int& width, const int& height)
{
	this->width = width;
	this->height = height;

	SDL_SetWindowSize(this->window, width, height);
}
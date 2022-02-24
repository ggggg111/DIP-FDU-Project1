#include "SDL.h"
#include "imgui_impl_sdl.h"

#include "Application.h"
#include "Input.h"

Input::Input()
{

}

Input::~Input()
{

}

void Input::Start()
{

}

void Input::PreUpdate()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		if (e.type == SDL_QUIT)
		{
			App->running = false;
		}
	}
}

void Input::Update()
{

}

void Input::PostUpdate()
{

}

void Input::CleanUp()
{

}
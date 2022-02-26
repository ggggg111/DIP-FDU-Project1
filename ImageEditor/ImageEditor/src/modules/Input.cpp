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
	memset(this->mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * NUM_MOUSE_BUTTONS);
}

void Input::PreUpdate()
{
	this->UpdateMouseState();

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch (e.type)
		{
			case SDL_QUIT:
			{
				App->running = false;
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				this->mouse_buttons[e.button.button - 1] = KEY_DOWN;
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				this->mouse_buttons[e.button.button - 1] = KEY_UP;
				break;
			}
			default:
			{
				break;
			}
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

KEY_STATE Input::GetMouseButton(const int& id)
{
	return this->mouse_buttons[id - 1];
}

void Input::GetMousePosition(int* x, int* y)
{
	SDL_GetMouseState(x, y);
}

void Input::UpdateMouseState()
{
	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (this->mouse_buttons[i] == KEY_DOWN)
		{
			this->mouse_buttons[i] = KEY_REPEAT;
		}

		if (this->mouse_buttons[i] == KEY_UP)
		{
			this->mouse_buttons[i] = KEY_IDLE;
		}
	}
}
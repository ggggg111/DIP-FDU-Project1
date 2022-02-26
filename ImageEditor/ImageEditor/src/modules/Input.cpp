#include <iostream>

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
	memset(this->mouse_buttons, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * NUM_MOUSE_BUTTONS);
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
				this->mouse_buttons[e.button.button - 1] = KEY_STATE::KEY_DOWN;
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				this->mouse_buttons[e.button.button - 1] = KEY_STATE::KEY_UP;
				break;
			}
			case SDL_MOUSEMOTION:
			{
				mouse_position_x = e.motion.x;
				mouse_position_y = e.motion.y;

				mouse_motion_x = e.motion.xrel;
				mouse_motion_y = e.motion.yrel;

				printf("Mouse position | x: %d y: %d\n", mouse_position_x, mouse_position_y);
				printf("Mouse motion | x: %d y: %d\n", mouse_motion_x, mouse_motion_y);

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

void Input::GetMousePosition(int& x, int& y)
{
	x = mouse_position_x;
	y = mouse_position_y;
}

void Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

void Input::UpdateMouseState()
{
	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (this->mouse_buttons[i] == KEY_STATE::KEY_REPEAT)
		{
			this->mouse_buttons[i] = KEY_STATE::KEY_REPEAT;
		}

		if (this->mouse_buttons[i] == KEY_STATE::KEY_REPEAT)
		{
			this->mouse_buttons[i] = KEY_STATE::KEY_REPEAT;
		}
	}
}
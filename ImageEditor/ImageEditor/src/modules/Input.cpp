#include <iostream>

#include "SDL.h"
#include "imgui_impl_sdl.h"

#include "Application.h"
#include "Input.h"
#include "Editor.h"
#include "Renderer.h"

Input::Input() : Module()
{

}

Input::~Input()
{

}

void Input::Start()
{
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		printf("SDL events can't be initialized. SDL_GetError: %s\n", SDL_GetError());
	}

	memset(this->mouse_buttons, (int)KEY_STATE::KEY_IDLE, sizeof(KEY_STATE) * NUM_MOUSE_BUTTONS);
}

void Input::PreUpdate()
{
	this->UpdateMouseState();

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse)
		{
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
					printf("Mouse button %d up(%d)\n", e.button.button - 1, KEY_STATE::KEY_UP);

					break;
				}
				case SDL_MOUSEMOTION:
				{
					this->mouse_position_x = e.motion.x;
					this->mouse_position_y = e.motion.y;

					this->mouse_motion_x = e.motion.xrel;
					this->mouse_motion_y = e.motion.yrel;

					break;
				}
				case SDL_DROPFILE:
				{
					std::string dropped_file_path = e.drop.file;
					printf("User loaded file %s\n", dropped_file_path.c_str());

					App->editor->bg = App->editor->LoadImg(dropped_file_path);
					App->editor->RenderImg(App->editor->bg, App->renderer->texture_target);

					break;
				}
				default:
				{
					break;
				}
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
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

KEY_STATE Input::GetMouseButton(const int& id) const
{
	return this->mouse_buttons[id - 1];
}

void Input::GetMousePosition(int& x, int& y) const
{
	x = mouse_position_x;
	y = mouse_position_y;
}

void Input::GetMouseMotion(int& x, int& y) const
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

void Input::UpdateMouseState()
{
	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (this->mouse_buttons[i] == KEY_STATE::KEY_DOWN)
		{
			this->mouse_buttons[i] = KEY_STATE::KEY_REPEAT;
		}

		if (this->mouse_buttons[i] == KEY_STATE::KEY_UP)
		{
			this->mouse_buttons[i] = KEY_STATE::KEY_IDLE;
		}
	}
}
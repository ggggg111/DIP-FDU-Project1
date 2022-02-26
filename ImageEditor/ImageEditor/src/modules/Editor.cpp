#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "portable-file-dialogs.h"

#include "tools/ImageLoader.h"
#include "Application.h"
#include "Editor.h"
#include "Renderer.h"
#include "Window.h"
#include "Input.h"

Editor::Editor()
	: bg(nullptr)
{

}

Editor::~Editor()
{

}

void Editor::Start()
{
	SDL_SetRenderTarget(App->renderer->renderer, App->renderer->texture_target);

	SDL_Rect rect_screen = { 0, 0, App->window->width, App->window->height };
	SDL_SetRenderDrawColor(App->renderer->renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(App->renderer->renderer, &rect_screen);

	SDL_SetRenderTarget(App->renderer->renderer, nullptr);
}

void Editor::Update()
{
	int mouse_position_x, mouse_position_y;
	App->input->GetMousePosition(mouse_position_x, mouse_position_y);

	int mouse_motion_x, mouse_motion_y;
	App->input->GetMouseMotion(mouse_motion_x, mouse_motion_y);

	SDL_SetRenderTarget(App->renderer->renderer, App->renderer->texture_target);
	
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN
		|| App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT)
	{
		SDL_SetRenderDrawColor(App->renderer->renderer,
			this->tools.GetColor().x,
			this->tools.GetColor().y,
			this->tools.GetColor().z,
			this->tools.GetColor().w
		);

		SDL_RenderDrawLine(
			App->renderer->renderer,
			mouse_position_x - mouse_motion_x, mouse_position_y - mouse_motion_y,
			mouse_position_x, mouse_position_y
		);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_DOWN
		|| App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_REPEAT)
	{
		SDL_SetRenderDrawColor(App->renderer->renderer, 255, 255, 255, 255);

		App->renderer->DrawCircle(mouse_position_x, mouse_position_y, this->tools.tool_size);
	}
	
	SDL_SetRenderTarget(App->renderer->renderer, nullptr);
}

void Editor::CleanUp()
{
	SDL_DestroyTexture(this->bg);
}

void Editor::DrawGUI()
{
	bool imgui_demo = true;
	ImGui::ShowDemoWindow(&imgui_demo);

	this->MainMenuBar();
	this->ToolSelection();
}

void Editor::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load image"))
			{
				auto selection = pfd::open_file("Select a file", ".",
					{ "Image Files", "*.png *.jpg *.bmp" })
					.result();

				if (!selection.empty())
				{
					std::string path = selection[0];
					printf("User selected file %s\n", path.c_str());
					this->bg= this->LoadImg(path);

					SDL_SetRenderTarget(App->renderer->renderer, App->renderer->texture_target);

					if (this->bg)
						SDL_RenderCopy(App->renderer->renderer, this->bg, nullptr, nullptr);

					SDL_SetRenderTarget(App->renderer->renderer, nullptr);
				}
			}

			if (ImGui::MenuItem("Save image"))
			{
				auto destination = pfd::save_file("Select a file", ".",
					{ "Image Files", "*.png *.jpg *.bmp" },
					pfd::opt::force_overwrite).result();

				printf("User selected file %s\n", destination.c_str());
				this->SaveImg(App->renderer->texture_target, destination);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Editor::ToolSelection()
{
	ImGui::Begin("Tools");
	ImGui::Text("Tool selection");
	ImGui::ColorEdit4("Current color", (float*)&this->tools.GetColorReference());
	ImGui::DragInt("Tool size", &this->tools.tool_size, 1, 1, 100, "%d%", ImGuiSliderFlags_AlwaysClamp);

	ImGui::End();
}

SDL_Texture* Editor::LoadImg(const std::string& path)
{
	SDL_Texture* t = ImageLoader::LoadTexture(App->renderer->renderer, path);
	
	int width, height;
	ImageLoader::GetTextureDimensions(t, &width, &height);

	App->renderer->texture_target = SDL_CreateTexture(
		App->renderer->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		width, height);

	App->window->SetWindowSize(width, height);

	return t;
}

void Editor::SaveImg(SDL_Texture* texture, const std::string& path)
{
	SDL_SetRenderTarget(App->renderer->renderer, texture);
	ImageLoader::SaveTexture(App->renderer->renderer, texture, path);
}
#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "portable-file-dialogs.h"
#include "SDL2_gfxPrimitives.h"

#include "tools/ImageLoader.h"
#include "Application.h"
#include "Editor.h"
#include "Renderer.h"
#include "Window.h"
#include "Input.h"

Editor::Editor()
	: Module(), bg(nullptr)
{

}

Editor::~Editor()
{

}

void Editor::Start()
{
	App->input->GetMousePosition(this->mouse_position_x, this->mouse_position_y);
	App->input->GetMousePosition(this->last_frame_mouse_position_x, this->last_frame_mouse_position_y);
}

void Editor::Update()
{
	App->input->GetMousePosition(this->mouse_position_x, this->mouse_position_y);

	App->renderer->SetRenderTarget(App->renderer->texture_target);

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_DOWN
		|| App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_STATE::KEY_REPEAT)
	{
		App->renderer->SetRenderDrawColor(
			this->tools.GetColor().x,
			this->tools.GetColor().y,
			this->tools.GetColor().z,
			255
		);

		thickLineRGBA(
			App->renderer->renderer,
			this->last_frame_mouse_position_x,
			this->last_frame_mouse_position_y,
			this->mouse_position_x,
			this->mouse_position_y,
			this->tools.tool_size,
			this->tools.GetColor().x,
			this->tools.GetColor().y,
			this->tools.GetColor().z,
			255
		);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_DOWN
		|| App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_REPEAT)
	{
		App->renderer->SetRenderDrawColor(255, 255, 255, 255);

		App->renderer->DrawCircleFill(mouse_position_x, mouse_position_y, this->tools.tool_size);
	}
	
	App->renderer->SetRenderTarget(nullptr);

	this->last_frame_mouse_position_x = this->mouse_position_x;
	this->last_frame_mouse_position_y = this->mouse_position_y;
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

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About"))
			{
				App->RequestBrowser("https://github.com/nngg11/DIP-FDU-Project1");
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

SDL_Texture* Editor::LoadImg(const std::string& path) const
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

void Editor::SaveImg(SDL_Texture* texture, const std::string& path) const
{
	SDL_SetRenderTarget(App->renderer->renderer, texture);
	ImageLoader::SaveTexture(App->renderer->renderer, texture, path);
}
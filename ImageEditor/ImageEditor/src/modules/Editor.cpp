#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "portable-file-dialogs.h"

#include "tools/ImageLoader.h"
#include "Application.h"
#include "Editor.h"
#include "Renderer.h"
#include "Window.h"

Editor::Editor()
	: texture(nullptr)
{

}

Editor::~Editor()
{

}

void Editor::Start()
{
	this->texture = App->renderer->texture_target;
	this->LoadImg("images/test3.jpg");
}

void Editor::Update()
{
	SDL_SetRenderTarget(App->renderer->renderer, this->texture);
	SDL_RenderCopy(App->renderer->renderer, this->texture, nullptr, nullptr);
	SDL_SetRenderTarget(App->renderer->renderer, nullptr);
	SDL_RenderCopy(App->renderer->renderer, this->texture, nullptr, nullptr);
}

void Editor::CleanUp()
{

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
					this->LoadImg(path);
				}
			}

			if (ImGui::MenuItem("Save image"))
			{
				auto destination = pfd::save_file("Select a file", ".",
					{ "Image Files", "*.png *.jpg *.bmp" },
					pfd::opt::force_overwrite).result();

				printf("User selected file %s\n", destination.c_str());
				this->SaveImg(this->texture, destination);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Editor::LoadImg(const std::string& path)
{
	this->texture = ImageLoader::LoadTexture(App->renderer->renderer, path);
	
	int width, height;
	ImageLoader::GetTextureDimensions(this->texture, &width, &height);

	App->window->SetWindowSize(width, height);
}

void Editor::SaveImg(SDL_Texture* texture, const std::string& path)
{
	ImageLoader::SaveTexture(App->renderer->renderer, texture, path);
}
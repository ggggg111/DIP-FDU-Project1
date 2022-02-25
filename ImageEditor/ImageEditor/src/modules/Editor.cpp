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
{

}

Editor::~Editor()
{

}

void Editor::Start()
{
	//App->renderer->texture_target = this->LoadImg("images/test2.png");
}

void Editor::Update()
{
	SDL_SetRenderTarget(App->renderer->renderer, App->renderer->texture_target);

	SDL_Rect rect_screen = { 0, 0, App->window->width, App->window->height };
	SDL_RenderFillRect(App->renderer->renderer, &rect_screen);

	SDL_RenderCopy(App->renderer->renderer, App->renderer->texture_target, nullptr, nullptr);
	
	SDL_SetRenderTarget(App->renderer->renderer, nullptr);
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
				this->SaveImg(App->renderer->texture_target, destination);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

SDL_Texture* Editor::LoadImg(const std::string& path)
{
	SDL_Texture* t = ImageLoader::LoadTexture(App->renderer->renderer, path);
	
	int width, height;
	ImageLoader::GetTextureDimensions(t, &width, &height);

	App->window->SetWindowSize(width, height);

	return t;
}

void Editor::SaveImg(SDL_Texture* texture, const std::string& path)
{
	SDL_SetRenderTarget(App->renderer->renderer, texture);
	ImageLoader::SaveTexture(App->renderer->renderer, texture, path);
}
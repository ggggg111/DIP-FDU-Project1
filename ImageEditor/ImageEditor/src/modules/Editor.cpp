#include "SDL.h"
#include "imgui.h"
#include "portable-file-dialogs.h"

#include "tools/ImageLoader.h"
#include "Application.h"
#include "Editor.h"
#include "Renderer.h"
#include "Window.h"

Editor::Editor()
	: img(nullptr)
{

}

Editor::~Editor()
{

}

void Editor::Start()
{
	this->img = App->window->GetWindowTexture(App->renderer->renderer);
}

void Editor::Update()
{
	this->MainMenuBar();

	SDL_RenderCopy(App->renderer->renderer, this->img, nullptr, nullptr);
}

void Editor::CleanUp()
{
	if (this->img)
	{
		SDL_DestroyTexture(this->img);
	}
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

			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Editor::LoadImg(const std::string& path)
{
	this->img = ImageLoader::LoadTexture(App->renderer->renderer, path);
	
	int width, height;
	ImageLoader::GetTextureDimensions(this->img, &width, &height);

	App->window->SetWindowSize(width, height);
}
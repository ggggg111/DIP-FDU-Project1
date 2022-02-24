#include "SDL.h"
#include "imgui.h"
#include "portable-file-dialogs.h"

#include "tools/ImageLoader.h"
#include "Application.h"
#include "Editor.h"
#include "Renderer.h"

Editor::Editor()
	: img(nullptr)
{

}

Editor::~Editor()
{

}

void Editor::Start()
{
	img = ImageLoader::LoadTexture(App->renderer->renderer, "images/test.jpg");
}

void Editor::Update()
{
	this->MainMenuBar();

	SDL_RenderCopy(App->renderer->renderer, img, nullptr, nullptr);
}

void Editor::CleanUp()
{
	SDL_DestroyTexture(img);
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
					printf("User selected file %s\n", selection[0].c_str());
			}

			if (ImGui::MenuItem("Save image"))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
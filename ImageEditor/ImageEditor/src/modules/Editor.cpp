#include "SDL.h"
#include "imgui.h"
#include "nfd.h"

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
				nfdchar_t* outPath = nullptr;
				nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);

				if (result == NFD_OKAY)
				{
					puts("Success!");
					puts(outPath);
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
					puts("User pressed cancel.");
				}
				else
				{
					printf("Error: %s\n", NFD_GetError());
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
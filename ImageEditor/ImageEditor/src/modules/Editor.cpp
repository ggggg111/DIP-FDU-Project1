#include "SDL.h"
#include "SDL_image.h"
#include "imgui.h"
#include "portable-file-dialogs.h"

#include "tools/ImageLoader.h"
#include "tools/Filters.h"
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
			this->tools.GetColor().w
		);

		switch (this->tools.current_tool)
		{
			case TOOLS::STANDARD_BRUSH:
			{
				this->UseStandardBrush();

				break;
			}
			case TOOLS::RUBBER:
			{
				this->UseRubber();

				break;
			}
			case TOOLS::CIRCLE_BRUSH:
			{
				this->UseCirleBrush();

				break;
			}
			case TOOLS::CIRCLE_BRUSH_FILL:
			{
				this->UseCirleBrushFill();

				break;
			}
			default:
			{
				break;
			}
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_DOWN
		|| App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_REPEAT)
	{
		this->UseRubber();
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
					printf("User loaded file %s\n", path.c_str());

					this->bg= this->LoadImg(path);
					this->RenderImg(App->renderer->renderer, this->bg, App->renderer->texture_target);
				}
			}

			if (ImGui::MenuItem("Save image"))
			{
				auto destination = pfd::save_file("Select a file", ".",
					{ "Image Files", "*.png *.jpg *.bmp" },
					pfd::opt::force_overwrite).result();

				this->SaveImg(App->renderer->texture_target, destination);
				printf("User saved file %s\n", destination.c_str());
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Image"))
		{
			if (ImGui::BeginMenu("Filters"))
			{
				if (ImGui::MenuItem("Grayscale"))
				{
					Filters::ApplyGrayScale(App->renderer->texture_target, App->renderer->texture_filter);
				}

				if (ImGui::MenuItem("Blur"))
				{
					Filters::ApplyBlur(App->renderer->texture_target, App->renderer->texture_filter, 3);
				}

				ImGui::EndMenu();
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

	static const char* items[4] = { "Standard Brush", "Rubber", "Circle Brush", "Circle Brush Fill"};
	ImGui::Combo("Tool", (int*)&this->tools.current_tool, items, IM_ARRAYSIZE(items));
	ImGui::ColorEdit4("Color", (float*)&this->tools.GetColorReference());
	ImGui::SliderInt("Size", &this->tools.tool_size, 1, 100, "%d", ImGuiSliderFlags_AlwaysClamp);

	ImGui::End();
}

void Editor::UseStandardBrush()
{
	App->renderer->DrawLine(
		this->last_frame_mouse_position_x, this->last_frame_mouse_position_y,
		this->mouse_position_x, this->mouse_position_y,
		this->tools.tool_size,
		this->tools.GetColor()
	);
}

void Editor::UseRubber()
{
	App->renderer->DrawCircleFill(
		mouse_position_x, mouse_position_y, 
		this->tools.tool_size,
		ImVec4(255, 255, 255, 255)
	);
}

void Editor::UseCirleBrush()
{
	App->renderer->DrawCircle(
		mouse_position_x, mouse_position_y,
		this->tools.tool_size,
		this->tools.GetColor()
	);
}

void Editor::UseCirleBrushFill()
{
	App->renderer->DrawCircleFill(
		mouse_position_x, mouse_position_y,
		this->tools.tool_size,
		this->tools.GetColor()
	);
}

SDL_Texture* Editor::LoadImg(const std::string& path) const
{
	SDL_Texture* t = ImageLoader::LoadTexture(App->renderer->renderer, path);
	
	int width, height;
	ImageLoader::GetTextureDimensions(t, &width, &height);

	App->renderer->texture_filter = SDL_CreateTexture(
		App->renderer->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height
	);

	App->renderer->texture_target = SDL_CreateTexture(
		App->renderer->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		width, height
	);

	App->window->SetWindowSize(width, height);

	return t;
}

void Editor::SaveImg(SDL_Texture* texture, const std::string& path) const
{
	SDL_SetRenderTarget(App->renderer->renderer, texture);
	ImageLoader::SaveTexture(App->renderer->renderer, texture, path);
}

void Editor::RenderImg(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Texture* target)
{
	SDL_SetRenderTarget(renderer, target);

	if (texture)
	{
		if (SDL_RenderCopy(renderer, texture, nullptr, nullptr) != 0)
		{
			printf("Render copy can't be performed. SDL_GetErrr: %s\n", SDL_GetError());
		}
	}

	SDL_SetRenderTarget(renderer, nullptr);
}
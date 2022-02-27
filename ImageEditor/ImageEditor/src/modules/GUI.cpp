#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "Application.h"
#include "GUI.h"
#include "Window.h"
#include "Renderer.h"
#include "Editor.h"

GUI::GUI() : Module()
{

}

GUI::~GUI()
{

}

void GUI::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForSDLRenderer(App->window->window, App->renderer->renderer);
	ImGui_ImplSDLRenderer_Init(App->renderer->renderer);
}

void GUI::CleanUp()
{
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void GUI::Draw()
{
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	App->editor->DrawGUI();

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}
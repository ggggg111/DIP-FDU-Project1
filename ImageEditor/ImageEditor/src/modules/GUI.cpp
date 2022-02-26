#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "Application.h"
#include "GUI.h"
#include "Window.h"
#include "Renderer.h"
#include "Editor.h"

GUI::GUI()
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

void GUI::PreUpdate()
{
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void GUI::Update()
{
	App->editor->DrawGUI();
}

void GUI::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

void GUI::CleanUp()
{
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
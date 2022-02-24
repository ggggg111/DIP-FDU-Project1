#include "SDL.h"

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
	SDL_RenderCopy(App->renderer->renderer, img, nullptr, nullptr);
}

void Editor::CleanUp()
{
	SDL_DestroyTexture(img);
}
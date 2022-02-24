#include "SDL.h"

#include "tools/ImageLoader.h"
#include "Editor.h"

Editor::Editor()
{

}

Editor::~Editor()
{

}

void Editor::Start()
{
	SDL_Surface* img = ImageLoader::LoadImage("path");
}

void Editor::Update()
{

}

void Editor::CleanUp()
{

}
#include <stdio.h>

#include "SDL.h"

#include "Application.h"
#include "modules/Window.h"
#include "modules/Renderer.h"
#include "utils/MemLeaks.h"

Application* App = nullptr;

int main(int argc, char* args[])
{
	App = new Application();

	App->Run();

	delete App;

	ReportMemoryLeaks();

	return 0;
}
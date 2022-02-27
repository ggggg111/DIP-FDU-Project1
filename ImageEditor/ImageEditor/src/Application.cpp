#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include "Application.h"
#include "modules/Window.h"
#include "modules/Renderer.h"
#include "modules/GUI.h"
#include "modules/Input.h"
#include "modules/Editor.h"

Application::Application()
    : running(true)
{
    this->AssignPlatform();

    this->window = new Window("Image Editor", 1024, 768);
    this->renderer = new Renderer();
    this->gui = new GUI();
    this->input = new Input();
    this->editor = new Editor();
}

Application::~Application()
{
    delete this->editor;
    delete this->input;
    delete this->gui;
    delete this->renderer;
    delete this->window;
}

void Application::Run()
{
    this->Start();

    while (this->running)
    {
        this->PreUpdate();
        this->Update();
        this->PostUpdate();
    }

    this->CleanUp();
}

void Application::Start()
{
    this->window->Start();
    this->renderer->Start();
    this->gui->Start();
    this->input->Start();
    this->editor->Start();
}

void Application::PreUpdate()
{
    this->input->PreUpdate();
    this->renderer->PreUpdate();
}

void Application::Update()
{
    this->input->Update();
    this->editor->Update();
    this->renderer->Update();
}

void Application::PostUpdate()
{
    this->input->PostUpdate();
    this->renderer->PostUpdate();
}

void Application::CleanUp()
{
    this->editor->CleanUp();
    this->input->CleanUp();
    this->gui->CleanUp();
    this->renderer->CleanUp();
    this->window->CleanUp();
}

void Application::AssignPlatform()
{
#ifdef _WIN32
    this->platform = PLATFORM::WINDOWS_32;
#elif _WIN64
    this->platform = PLATFORM::WINDOWS_64;
#elif __linux__
    this->platform = PLATFORM::LINUX;
#elif __APPLE__
    this->platform = PLATFORM::APPLE;
#else
    this->platform = PLATFORM::UNKNOWN;
#endif
}

void Application::RequestBrowser(const std::string& url)
{
    switch (App->platform)
    {
        case PLATFORM::WINDOWS_32:
        case PLATFORM::WINDOWS_64:
        {
            ShellExecuteA(0, "open", url.c_str(), 0, 0, SW_SHOWNORMAL);

            break;
        }
        case PLATFORM::LINUX:
        {
            std::system(std::string("xdg-open").append(" ").append(url).c_str());

            break;
        }
        case PLATFORM::APPLE:
        {
            std::system(std::string("open").append(" ").append(url).c_str());

            break;
        }
        default:
        {
            break;
        }
    }
}
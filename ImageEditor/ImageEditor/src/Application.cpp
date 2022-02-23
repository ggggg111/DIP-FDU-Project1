#include "Application.h"
#include "modules/Window.h"
#include "modules/Renderer.h"
#include "modules/GUI.h"
#include "modules/Input.h"

Application::Application()
    : running(true)
{
    window = new Window("Image Editor", 1024, 768);
    renderer = new Renderer();
    gui = new GUI();
    input = new Input();
}

Application::~Application()
{
    delete input;
    delete gui;
    delete renderer;
    delete window;
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
    window->Start();
    renderer->Start();
    gui->Start();
    input->Start();
}

void Application::PreUpdate()
{
    input->PreUpdate();
    gui->PreUpdate();
    renderer->PreUpdate();
}

void Application::Update()
{
    input->Update();
    gui->Update();
    renderer->Update();
}

void Application::PostUpdate()
{
    input->PostUpdate();
    gui->PostUpdate();
    renderer->PostUpdate();
}

void Application::CleanUp()
{
    input->CleanUp();
    gui->CleanUp();
    renderer->CleanUp();
    window->CleanUp();
}
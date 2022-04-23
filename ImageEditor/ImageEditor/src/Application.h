#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <iostream>
#include <list>

class Module;
class Window;
class Renderer;
class GUI;
class Input;
class Editor;
class TorchLoader;

class Application
{
public:
    Application();
    ~Application();

    void Run();

    void Start();
    void Update();
    void CleanUp();

    void RequestBrowser(const std::string& url) const;

private:
    void AddModule(Module* module);

public:
    Window* window;
    Renderer* renderer;
    GUI* gui;
    Input* input;
    Editor* editor;
    TorchLoader* torch_loader;

    bool running;
    std::string name;

private:
    std::list<Module*> modules;
};

extern Application* App;

#endif /* __APPLICATION_H__ */
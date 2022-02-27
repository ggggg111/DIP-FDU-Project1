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

enum class PLATFORM
{
    WINDOWS_32 = 0,
    WINDOWS_64,
    LINUX,
    APPLE,

    UNKNOWN = -1
};

class Application
{
public:
    Application();
    ~Application();

    void Run();

    void Start();
    void Update();
    void CleanUp();

    void RequestBrowser(const std::string& url);

private:
    void AssignPlatform();
    void AddModule(Module* module);

public:
    PLATFORM platform;

    Window* window;
    Renderer* renderer;
    GUI* gui;
    Input* input;
    Editor* editor;

    bool running;

private:
    std::list<Module*> modules;
};

extern Application* App;

#endif /* __APPLICATION_H__ */
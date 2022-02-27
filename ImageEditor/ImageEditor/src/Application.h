#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <iostream>

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

    void PreUpdate();
    void Update();
    void PostUpdate();

    void CleanUp();

    void RequestBrowser(const std::string& url);

private:
    void AssignPlatform();

public:
    PLATFORM platform;

    Window* window;
    Renderer* renderer;
    GUI* gui;
    Input* input;
    Editor* editor;

    bool running;
};

extern Application* App;

#endif /* __APPLICATION_H__ */
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

class Window;
class Renderer;
class GUI;
class Input;
class Editor;

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

public:
    Window* window;
    Renderer* renderer;
    GUI* gui;
    Input* input;
    Editor* editor;

    bool running;
};

extern Application* App;

#endif /* __APPLICATION_H__ */
#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <iostream>

struct SDL_Window;

class Window
{
public:
	Window(const std::string& name, const int& width, const int& height);
	~Window();

	void Start();
	void CleanUp();

public:
	SDL_Window* window;

private:
	std::string name;
	int width;
	int height;
};

#endif /* __WINDOW_H__ */
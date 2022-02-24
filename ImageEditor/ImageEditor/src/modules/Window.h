#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <iostream>

struct SDL_Window;
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;

class Window
{
public:
	Window(const std::string& name, const int& width, const int& height);
	~Window();

	void Start();
	void CleanUp();

	SDL_Texture* GetWindowTexture(SDL_Renderer* renderer) const;
	void SetWindowSize(const int& width, const int& height);

public:
	SDL_Window* window;
	SDL_Surface* surface;

private:
	std::string name;
	int width;
	int height;
};

#endif /* __WINDOW_H__ */
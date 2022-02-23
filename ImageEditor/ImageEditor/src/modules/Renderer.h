#ifndef __RENDERER_H__
#define __RENDERER_H__

struct SDL_Renderer;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Start();
	void PreUpdate();
	void Update();
	void PostUpdate();
	void CleanUp();

public:
	SDL_Renderer* renderer;
};

#endif /* __RENDERER_H__ */
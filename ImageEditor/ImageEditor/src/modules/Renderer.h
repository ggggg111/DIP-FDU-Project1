#ifndef __RENDERER_H__
#define __RENDERER_H__

struct SDL_Renderer;
struct SDL_Texture;

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

	void DrawCircle(const int& x, const int& y, const int& radius);
	void DrawCircleFill(const int& x, const int& y, const int& radius);

public:
	SDL_Renderer* renderer;
	SDL_Texture* texture_target;
};

#endif /* __RENDERER_H__ */
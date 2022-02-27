#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Module.h"

struct SDL_Renderer;
struct SDL_Texture;

class Renderer : public Module
{
public:
	Renderer();
	~Renderer();

	void Start() override;
	void PreUpdate() override;
	void Update() override;
	void PostUpdate() override;
	void CleanUp() override;

	void DrawCircle(const int& x, const int& y, const int& radius);
	void DrawCircleFill(const int& x, const int& y, const int& radius);

	void SetRenderTarget(SDL_Texture* texture) const;
	void SetRenderDrawColor(const int& r, const int& g, const int& b, const int& a) const;

public:
	SDL_Renderer* renderer;
	SDL_Texture* texture_target;
};

#endif /* __RENDERER_H__ */
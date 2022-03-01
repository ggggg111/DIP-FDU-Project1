#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "imgui.h"

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

	void DrawLine(const int& x1, const int& y1, const int& x2, const int& y2, const int& size, const ImVec4& color);
	void DrawCircle(const int& x, const int& y, const int& radius, const ImVec4& color);
	void DrawCircleFill(const int& x, const int& y, const int& radius, const ImVec4& color);

	void SetRenderTarget(SDL_Texture* texture) const;
	void SetRenderDrawColor(const int& r, const int& g, const int& b, const int& a) const;

public:
	SDL_Renderer* renderer;
	SDL_Texture* texture_target;
	SDL_Texture* texture_filter;
};

#endif /* __RENDERER_H__ */
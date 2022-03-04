#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "SDL.h"
#include "imgui.h"

#include "Module.h"

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

	SDL_Texture* CreateTexture(const Uint32& format, const int& access, const int& width, const int& height);

	void RenderTexture(SDL_Texture* texture, SDL_Rect* source_rect, SDL_Rect* destination_rect);

public:
	SDL_Renderer* renderer;
	SDL_Texture* texture_target;
	SDL_Texture* texture_filter;
	SDL_Texture* texture_workbench_target;

	int texture_target_width;
	int texture_target_height;

	Uint32 texture_format;
};

#endif /* __RENDERER_H__ */
#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "tools/EditorToolSelector.h"
#include "Module.h"

struct SDL_Texture;
struct SDL_Rect;

class Editor : public Module
{
public:
	Editor();
	~Editor();

	void Start() override;
	void Update() override;
	void CleanUp() override;

public:
	void DrawGUI();

	SDL_Texture* LoadImg(const std::string& path) const;
	void SaveImg(SDL_Texture* texture, const std::string& path) const;

	void RenderImg(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Texture* target);

private:
	void MainMenuBar();
	void ToolSelection();

	void UseStandardBrush();
	void UseRubber();
	void UseCirleBrush();
	void UseCirleBrushFill();

public:
	EditorToolSelector tools;
	SDL_Texture* bg;
	SDL_Rect bg_rect;

private:
	int mouse_position_x;
	int mouse_position_y;

	int last_frame_mouse_position_x;
	int last_frame_mouse_position_y;
};

#endif /* __EDITOR_H__ */
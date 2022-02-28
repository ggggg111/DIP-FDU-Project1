#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "tools/EditorToolSelector.h"
#include "Module.h"

struct SDL_Texture;

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

private:
	SDL_Texture* LoadImg(const std::string& path) const;
	void SaveImg(SDL_Texture* texture, const std::string& path) const;

	void MainMenuBar();
	void ToolSelection();

	void UseBrush();
	void UseRubber();

public:
	EditorToolSelector tools;

private:
	SDL_Texture* bg;

	int mouse_position_x;
	int mouse_position_y;

	int last_frame_mouse_position_x;
	int last_frame_mouse_position_y;
};

#endif /* __EDITOR_H__ */
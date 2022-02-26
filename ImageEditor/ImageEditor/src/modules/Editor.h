#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "tools/EditorToolSelector.h"

struct SDL_Texture;

class Editor
{
public:
	Editor();
	~Editor();

	void Start();
	void Update();
	void CleanUp();

public:
	void DrawGUI();

private:
	SDL_Texture* LoadImg(const std::string& path);
	void SaveImg(SDL_Texture* texture, const std::string& path);

	void MainMenuBar();
	void ToolSelection();

public:
	EditorToolSelector tools;

private:
	SDL_Texture* bg;
};

#endif /* __EDITOR_H__ */
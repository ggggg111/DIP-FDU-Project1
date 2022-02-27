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
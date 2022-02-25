#ifndef __EDITOR_H__
#define __EDITOR_H__

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
	void MainMenuBar();

private:
	SDL_Texture* LoadImg(const std::string& path);
	void SaveImg(SDL_Texture* texture, const std::string& path);
};

#endif /* __EDITOR_H__ */
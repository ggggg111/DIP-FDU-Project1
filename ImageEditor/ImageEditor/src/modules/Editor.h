#ifndef __EDITOR_H__
#define __EDITOR_H__

struct SDL_Surface;
struct SDL_Texture;

class Editor
{
public:
	Editor();
	~Editor();

	void Start();
	void Update();
	void CleanUp();

private:
	void MainMenuBar();
	void LoadImg(const std::string& path);

private:
	SDL_Texture* texture;
};

#endif /* __EDITOR_H__ */
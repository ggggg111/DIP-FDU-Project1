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

private:
	SDL_Texture* img;
};

#endif /* __EDITOR_H__ */
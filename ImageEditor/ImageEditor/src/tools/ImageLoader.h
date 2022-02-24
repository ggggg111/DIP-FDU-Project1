#include <iostream>

struct SDL_Texture;
struct SDL_Renderer;

class ImageLoader
{
public:
	static SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string& path);
	static void GetTextureDimensions(SDL_Texture* texture, int* width, int* height);
};
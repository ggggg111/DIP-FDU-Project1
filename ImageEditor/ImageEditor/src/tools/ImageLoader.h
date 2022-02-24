#include <iostream>

struct SDL_Surface;

class ImageLoader
{
public:
	static SDL_Surface* LoadImage(const std::string& path);
};
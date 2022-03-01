#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <iostream>

struct SDL_Texture;
struct SDL_Renderer;

class Filters
{
public:
	static void ApplyGrayScale(SDL_Texture* texture);
};

#endif /* __FILTERS_H__ */
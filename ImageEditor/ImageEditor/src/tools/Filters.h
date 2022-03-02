#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <iostream>

struct SDL_Texture;

class Filters
{
public:
	static void ApplyGrayScale(SDL_Texture* target, SDL_Texture* filter);
};

#endif /* __FILTERS_H__ */
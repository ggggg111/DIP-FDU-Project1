#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <iostream>

struct SDL_Texture;

class Filters
{
public:
	static void ApplyGrayScale(SDL_Texture* target, SDL_Texture* filter);
	static void ApplyBlur(SDL_Texture* target, SDL_Texture* filter, const int& kernel_size);
};

#endif /* __FILTERS_H__ */
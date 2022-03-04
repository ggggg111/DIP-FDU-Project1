#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <iostream>

#include "SDL.h"

class Filters
{
public:
	static void ApplyGrayScale(SDL_Texture* target, SDL_Texture* filter);
	static void ApplyBlur(SDL_Texture* target, SDL_Texture* filter, const int& kernel_size);
	static void ApplyNegative(SDL_Texture* target, SDL_Texture* filter);
};

#endif /* __FILTERS_H__ */
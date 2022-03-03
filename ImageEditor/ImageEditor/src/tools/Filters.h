#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <iostream>

#include "SDL.h"

class Filters
{
public:
	static void ApplyGrayScale(SDL_Texture* target, SDL_Texture* filter);
	static void ApplyBlur(SDL_Texture* target, SDL_Texture* filter, const int& kernel_size);

private:
	static Uint32 Convert1dTo2dArray(Uint32* array, const int& i, const int& j, const int& columns);
};

#endif /* __FILTERS_H__ */
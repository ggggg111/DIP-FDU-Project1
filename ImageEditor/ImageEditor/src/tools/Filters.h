#ifndef __FILTERS_H__
#define __FILTERS_H__

#include <iostream>
#include <vector>

struct SDL_Texture;

class Filters
{
public:
	static void ApplyGrayScale(SDL_Texture* target, SDL_Texture* filter);
	static void ApplyBlur(SDL_Texture* target, SDL_Texture* filter, const int& kernel_size);
	static void ApplyMedianBlur(SDL_Texture* target, SDL_Texture* filter, const int& kernel_size);
	static void ApplyNegative(SDL_Texture* target, SDL_Texture* filter);

private:
	static std::vector<float> CreateKernel(const int& kernel_size, const float& value);
};

#endif /* __FILTERS_H__ */
#ifndef __IMAGE_LOADER_H__
#define __IMAGE_LOADER_H__

#include <iostream>

#include "SDL.h"
#include "opencv2/imgcodecs.hpp"

struct SDL_Texture;
struct SDL_Renderer;

class ImageLoader
{
public:
	static SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string& path);
	static void SaveTexture(SDL_Renderer* renderer, SDL_Texture* texture, const std::string& path);
	
	static void GetTextureDimensions(SDL_Texture* texture, int* width, int* height);
	static void GetTextureInformation(SDL_Texture* texture, Uint32* format, int* access, int* width, int* height);

	static void SendMatToEditor(const cv::Mat& ldr);
};

#endif /* __IMAGE_LOADER_H__ */
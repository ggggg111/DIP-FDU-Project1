#include "SDL_image.h"

#include "ImageLoader.h"

SDL_Texture* ImageLoader::LoadTexture(SDL_Renderer* renderer, const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
        printf("Image %s can't be loaded. IMG_GetError: %s\n", path.c_str(), IMG_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        printf("Texture from %s can't be created. SDL_GetError: %s\n", path.c_str(), SDL_GetError());
    }

    SDL_FreeSurface(surface);

    return texture;
}
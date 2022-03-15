#include <iostream>
#include <filesystem>

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

void ImageLoader::SaveTexture(SDL_Renderer* renderer, SDL_Texture* texture, const std::string& path)
{
    std::string extension = std::filesystem::path(path).extension().string();

    SDL_Texture* target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, texture);

    int width, height;
    ImageLoader::GetTextureDimensions(texture, &width, &height);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, nullptr, surface->format->format, surface->pixels, surface->pitch);
    
    if (extension == ".bmp")
        SDL_SaveBMP(surface, path.c_str());
    else if (extension == ".png")
        IMG_SavePNG(surface, path.c_str());
    else if (extension == ".jpg")
        IMG_SaveJPG(surface, path.c_str(), 100);

    SDL_FreeSurface(surface);
    SDL_SetRenderTarget(renderer, target);
}

void ImageLoader::GetTextureDimensions(SDL_Texture* texture, int* width, int* height)
{
    SDL_QueryTexture(texture, nullptr, nullptr, width, height);
}

void ImageLoader::GetTextureInformation(SDL_Texture* texture, Uint32* format, int* access, int* width, int* height)
{
    SDL_QueryTexture(texture, format, access, width, height);
}
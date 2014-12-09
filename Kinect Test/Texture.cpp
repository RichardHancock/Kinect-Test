#include "Texture.h"
#include <iostream>

Texture::Texture(std::string filename, SDL_Renderer* renderer)
{	
	load(filename, renderer);
}
Texture::Texture(SDL_Surface* surface, SDL_Renderer* renderer)
{
	load(surface, renderer);
}
Texture::~Texture()
{
	SDL_DestroyTexture(texture);
}

bool Texture::load(std::string filename, SDL_Renderer* renderer)
{
	SDL_Surface *image = IMG_Load(filename.c_str());
	if (!image)
	{
		// We'll do a quick check here because it's easy to get filenames or directories wrong
		std::cout << "Can't find image named " << filename << " " << IMG_GetError() << std::endl;
		return false;
	}

	currentRenderer = renderer;
	Texture::createTexture(*image); //Need error check here
	SDL_FreeSurface(image);
	return true;
}

bool Texture::load(SDL_Surface* surface , SDL_Renderer* renderer)
{
	currentRenderer = renderer;
	Texture::createTexture(*surface); //Need error check here
	return true;
}

bool Texture::createTexture(SDL_Surface &image)
{
	texture = SDL_CreateTextureFromSurface(currentRenderer, &image);

	// This is done because a float* and int* conflict.
	int tempDimensionsX;
	int tempDimensionsY;
	SDL_QueryTexture(texture, NULL, NULL, &tempDimensionsX, &tempDimensionsY);
	
	dimensions.x = (float)tempDimensionsX;
	dimensions.y = (float)tempDimensionsY;

	return (texture ? true : false);
}

void Texture::draw(Vec2 pos)
{
	SDL_Rect destRect;

	destRect.x = (int)pos.x;
	destRect.y = (int)pos.y;

	// Query the texture to get its original width and height
	destRect.w = (int)dimensions.x;
	destRect.h = (int)dimensions.y;

	// Here we are telling the renderer to copy the texture memory to our screen,
	// at the position of the rectangle we specify
	SDL_RenderCopy(currentRenderer, texture, NULL, &destRect);
}

Vec2 Texture::getDimensions()
{
	return dimensions;
}

SDL_Renderer* Texture::getRenderer()
{
	return currentRenderer;
}

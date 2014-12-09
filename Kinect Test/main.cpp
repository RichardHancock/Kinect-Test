#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Texture.h"
#include "Kinect.h"

int init();
int main(int, char**);
void cleanup();

//Constants
const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;

//Globals
SDL_Window *window;
SDL_Renderer *renderer;

int init()
{
	int status = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) { status = -1; }

	if (TTF_Init() < 0)
	{
		status = -1;
		std::cout << "SDL_ttf init failed: " << TTF_GetError << std::endl;
	}

	window = SDL_CreateWindow("Spiffing Spaceships",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH, WIN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!window || !renderer) { status = -1; }

	if (status == -1)
	{
		std::cout << "Error occurred in init: " << SDL_GetError() << std::endl;
	}
	return status;
}

void cleanup()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char **argv)
{
	if (init() == -1)
	{
		return -1;
	}

	KinectInput kinect(false, RightHand);
	kinect.startTracking();

	Texture* cursor = new Texture("cursor.png", renderer);
	Vec2 cursorCenter(cursor->getDimensions().x / 2, cursor->getDimensions().y / 2);

	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			//Escape Key
			if ((e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE) || e.type == SDL_QUIT)
			{
				quit = true;
			}

			//Basic demo movement
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
				case SDLK_w:
					break;
				case SDLK_DOWN:
				case SDLK_s:
					break;
				case SDLK_LEFT:
				case SDLK_a:
					break;
				case SDLK_RIGHT:
				case SDLK_d:
					break;
				}
			}
		}
		
		//Update
		kinect.update();

		//Render
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);		
		
		//This is only so messy because of the conversion from 3D to 2D
		Vec2 handPos(kinect.getHandPos().x, kinect.getHandPos().y);
		float handDepth = kinect.getHandPos().z;

		Vec2 posToDraw = cursorCenter - handPos;
		cursor->draw(posToDraw);
		
		// Debug cursor point
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderDrawPoint(renderer, handPos.x, handPos.y);

		SDL_RenderPresent(renderer);

	}

	cleanup();
	return 0;
}
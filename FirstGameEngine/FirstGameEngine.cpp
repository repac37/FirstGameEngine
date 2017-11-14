// FirstGameEngine.cpp : Defines the entry point for the console application.
//

//$(ProjectDir)dev\include bra skit

#include "stdafx.h"
#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_image.h"



#define FPS 60 // frames per second


using namespace std;


int main(int argc, char** argv)
{

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		cerr << "SDL_Init-fel: " << SDL_GetError() << endl;
		SDL_Quit();
		exit(-1);
	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		cerr << "Faild to initalize SDL_image." << endl;
		SDL_Quit();
		exit(-1);
	}

	SDL_Window* win = SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		cerr << "Fel: " << SDL_GetError() << endl;
		SDL_Quit();
		exit(-1);
	}

	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		cerr << "Fel: " << SDL_GetError() << endl;
		SDL_DestroyWindow(win);
		SDL_Quit();
		exit(-1);
	}

	SDL_Surface* bgSurf = SDL_LoadBMP("../images/space.bmp");
	SDL_Texture* bgTx = SDL_CreateTextureFromSurface(ren, bgSurf);
	SDL_FreeSurface(bgSurf);

	SDL_Surface* spaceShipSurf = SDL_LoadBMP("../images/spaceShip.bmp");
	SDL_Rect spaceShipRect = {0,0,200,200};
	Uint32 white = SDL_MapRGB(spaceShipSurf->format, 255, 255, 255);
	SDL_SetColorKey(spaceShipSurf, true, white);
	SDL_Texture* spaceShipTx = SDL_CreateTextureFromSurface(ren, spaceShipSurf);
	SDL_FreeSurface(spaceShipSurf);

	SDL_Surface* pyramidSurf = IMG_Load("../images/ufo.png");
	SDL_Texture* pyramidTx = SDL_CreateTextureFromSurface(ren, pyramidSurf);
	SDL_Rect pyramidRect = { 400,100,150,150};
	SDL_FreeSurface(pyramidSurf);

	
	bool runOn = true;
	int varv = 0;
	int extraMove = 5;
	bool drag = false;

	const int tickInterval = 1000 / FPS;
	while (runOn) {
		Uint32 nextTick = SDL_GetTicks() + tickInterval;
		SDL_Event eve;
		while (SDL_PollEvent(&eve)) {
			switch (eve.type) {
			case SDL_QUIT: runOn = false; break;
			case SDL_KEYDOWN: 
				switch (eve.key.keysym.sym) {
				case SDLK_RIGHT: spaceShipRect.x += extraMove; break;
				case SDLK_LEFT: spaceShipRect.x -= extraMove; break;
				case SDLK_UP: spaceShipRect.y -= extraMove; break;
				case SDLK_DOWN: spaceShipRect.y += extraMove; break;
				} break;
			case SDL_MOUSEBUTTONDOWN: {
				SDL_Point p = { eve.button.x, eve.button.y };
				if (SDL_PointInRect(&p, &spaceShipRect))
					drag = true;
			} break;
			case SDL_MOUSEBUTTONUP: drag = false; break;
			case SDL_MOUSEMOTION:
				if (drag) {
					spaceShipRect.x += eve.motion.xrel;
					spaceShipRect.y += eve.motion.yrel;
				} break;
			} // switch end
		}

		varv++;
		//spaceShipRect.x++;


		SDL_RenderClear(ren); //gör rent		
		SDL_RenderCopy(ren, bgTx, NULL, NULL); //ritar background
		SDL_RenderCopy(ren, spaceShipTx, NULL, &spaceShipRect); // ritar uf.
		SDL_RenderCopy(ren, pyramidTx, NULL, &pyramidRect); // ritar uf.
		SDL_RenderPresent(ren); // Skriver ut på skärmen från buffert
		
		int delay = nextTick - SDL_GetTicks();
		if (delay > 0) {
			SDL_Delay(delay);
		}

	}// while end
	SDL_DestroyTexture(pyramidTx);
	SDL_DestroyTexture(spaceShipTx);
	SDL_DestroyTexture(bgTx);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	
	return 0;
}


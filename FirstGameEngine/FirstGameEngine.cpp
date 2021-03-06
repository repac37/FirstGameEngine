// FirstGameEngine.cpp : Defines the entry point for the console application.
//

//$(ProjectDir)dev\include bra skit

#include "stdafx.h"
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define FPS 60 // frames per second

using namespace std;

int main(int argc, char** argv)
{
	//SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		cerr << "SDL_Init-fel: " << SDL_GetError() << endl;
		SDL_Quit();
		exit(-1);
	}

	//TTF
	if (TTF_Init() == -1) {
		cerr << "Faild to initalize SDL_ttf" << endl;
		SDL_Quit();
		exit(-1);
	}
	TTF_Font* font = TTF_OpenFont("../Fonts/04B_08__.ttf", 36);

	//Mixer
	int channel = Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
	Mix_Volume(channel, 10);
	Mix_Chunk* musik = Mix_LoadWAV("../Sound/351433__zagi2__sweet-melody.wav");
	Mix_Chunk* pling = Mix_LoadWAV("../Sound/80921__justinbw__buttonchime02up.wav");

	//IMG
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		cerr << "Faild to initalize SDL_image." << endl;
		SDL_Quit();
		exit(-1);
	}

	// Window
	SDL_Window* win = SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		cerr << "Fel: " << SDL_GetError() << endl;
		SDL_Quit();
		exit(-1);
	}

	//Renderare
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		cerr << "Fel: " << SDL_GetError() << endl;
		SDL_DestroyWindow(win);
		SDL_Quit();
		exit(-1);
	}

	//Skapa object
	SDL_Surface* bgSurf = SDL_LoadBMP("../images/space.bmp");
	SDL_Texture* bgTx = SDL_CreateTextureFromSurface(ren, bgSurf);
	SDL_FreeSurface(bgSurf);

	SDL_Surface* spaceShipSurf = IMG_Load("../images/spaceShip.png");
	//Uint32 white = SDL_MapRGB(spaceShipSurf->format, 255, 255, 255);
	//SDL_SetColorKey(spaceShipSurf, true, white);
	SDL_Texture* spaceShipTx = SDL_CreateTextureFromSurface(ren, spaceShipSurf);
	SDL_Rect spaceShipRect = { 0,0,spaceShipSurf->w/2, spaceShipSurf->h/2 };
	SDL_FreeSurface(spaceShipSurf);

	SDL_Surface* pyramidSurf = IMG_Load("../images/spacePlane.png");
	SDL_Texture* pyramidTx = SDL_CreateTextureFromSurface(ren, pyramidSurf);
	SDL_Rect pyramidRect = { 400,100,150,150};
	SDL_FreeSurface(pyramidSurf);

	SDL_Surface* deathSurf = IMG_Load("../images/ufo.png");
	SDL_Texture* deathTx = SDL_CreateTextureFromSurface(ren, deathSurf);
	SDL_Rect deathRect = { 200,100,150,150 };
	SDL_FreeSurface(deathSurf);
	

	//Run Loop
	bool runOn = true;
	int varv = 0;
	int extraMove = 5;
	bool drag = false;

	const int tickInterval = 1000 / FPS;

	Mix_PlayChannel(-1, musik, -1);


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
				case SDLK_UP: spaceShipRect.y -= extraMove; 
					Mix_Volume(channel,spaceShipRect.y); 
					break;
				case SDLK_DOWN: spaceShipRect.y += extraMove; 
					Mix_Volume(channel, spaceShipRect.y);
					break;
				} break;
			case SDL_MOUSEBUTTONDOWN: {
				SDL_Point p = { eve.button.x, eve.button.y };
				if (SDL_PointInRect(&p, &spaceShipRect))
					drag = true;
				Mix_PlayChannel(-1, pling, 0);
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
		string varvStr = "Game loop: " +  to_string(varv);
		SDL_Color red = { 255,0,0 };
		SDL_Surface* varvSurf = TTF_RenderText_Solid(font, varvStr.c_str(), red);
		SDL_Texture* varvTx = SDL_CreateTextureFromSurface(ren, varvSurf);
		SDL_Rect varvRect = { 450,20, varvSurf->w,varvSurf->h };
		SDL_FreeSurface(varvSurf);
		//spaceShipRect.x++;

		// Rita
		SDL_RenderClear(ren); //gör rent		
		SDL_RenderCopy(ren, bgTx, NULL, NULL); //ritar background
		SDL_RenderCopy(ren, spaceShipTx, NULL, &spaceShipRect); // ritar uf.
		SDL_RenderCopy(ren, pyramidTx, NULL, &pyramidRect);
		
		SDL_RenderCopy(ren, varvTx, NULL, &varvRect);// ritar uf.
		
		SDL_RenderCopy(ren, deathTx, NULL, &deathRect);
		SDL_RenderPresent(ren); // Skriver ut på skärmen från buffert
		
		//tar bort texturer som updateras kontinueliet i loopen
		SDL_DestroyTexture(varvTx);
		
		
		//För dröjer gameloopen innan den startar om.
		int delay = nextTick - SDL_GetTicks();
		if (delay > 0) {
			SDL_Delay(delay);
		}

	}// while end

	// Clean and destory
	TTF_CloseFont(font);
	Mix_FreeChunk(musik);
	Mix_FreeChunk(pling);
	SDL_DestroyTexture(pyramidTx);
	SDL_DestroyTexture(spaceShipTx);
	SDL_DestroyTexture(deathTx);
	SDL_DestroyTexture(bgTx);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
	
	return 0;
}


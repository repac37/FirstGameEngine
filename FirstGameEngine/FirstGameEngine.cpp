// FirstGameEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <SDL.h>
using namespace std;


int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		cerr << "SDL_Init-fel: " << SDL_GetError() << endl;
		exit(-1);
	}

	SDL_Window* win = SDL_CreateWindow("Mitt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 500, SDL_WINDOW_SHOWN);
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

    return 0;
}


#include "methods.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

using namespace std;

TEXT::TEXT(int _x, int _y, int _size, string _fnt){

    x = _x;
    y = _y;
    fnt = TTF_OpenFont(_fnt.c_str(), _size);
    c = {0, 0, 0};


    coords[0].x = 0;
    coords[0].y = 0;
}

void TEXT::draw(SDL_Renderer *ren, string _text){

    sfc = TTF_RenderText_Solid(fnt, _text.c_str(), c);

    width = sfc->w;
    height = sfc->h;

    coords[0].w = width;
    coords[0].h = height;

    coords[1].x = x;
    coords[1].y = y;
    coords[1].w = width;
    coords[1].h = height;

    tx = SDL_CreateTextureFromSurface(ren, sfc);
    SDL_FreeSurface(sfc);
    SDL_RenderCopy(ren, tx, &coords[0], &coords[1]);

}

int TEXT::getWidth(){
    return width;
}

int TEXT::getHeight(){
    return height;
}


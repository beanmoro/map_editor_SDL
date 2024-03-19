#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

#ifndef METHODS_H_INCLUDED
#define METHODS_H_INCLUDED

using namespace std;

class TEXT{

    int x, y, width, height;

    SDL_Surface* sfc;
    SDL_Texture* tx;
    SDL_Rect coords[2];
    TTF_Font *fnt;
    SDL_Color c;

public:

    TEXT(int _x, int _y, int _size, string _fnt);
    void draw(SDL_Renderer *ren, string _text);

    int getWidth();
    int getHeight();

};


#endif // METHODS_H_INCLUDED

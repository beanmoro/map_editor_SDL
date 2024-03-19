#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

using namespace std;

class BUTTON{

    bool bState = false;
    TTF_Font* fnt0;
    SDL_Rect bRect[2];
    SDL_Surface *sfc;
    SDL_Texture *tex;
    bool altern = false;
    bool bState_alt = false;
    bool aux = false;


public:

    BUTTON( int _x, int _y,  int _w,  int _h, SDL_Renderer *ren, int _cx, int _cy, int _cw, int ch, string imageFile, bool _altern);
    void b_step(int _x, int _y, int _mx, int _my, bool keyState, int _w, int _h);
    void b_draw(SDL_Renderer *ren);
    bool b_getState();
    void b_changeState(bool _state);

};

#endif // INTERFACE_H_INCLUDED

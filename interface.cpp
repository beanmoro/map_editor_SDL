#include "interface.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

using namespace std;


BUTTON::BUTTON(int _x, int _y, int _w, int _h, SDL_Renderer *ren, int _cx, int _cy, int _cw, int _ch, string imageFile, bool _altern){

    bRect[1].x = _x;
    bRect[1].y = _y;
    bRect[1].w = _w;
    bRect[1].h = _h;

    bRect[0].x = _cx;
    bRect[0].y = _cy;
    bRect[0].w = _cw;
    bRect[0].h = _ch;

    altern = _altern;


    sfc = IMG_Load(imageFile.c_str());
    tex = SDL_CreateTextureFromSurface(ren, sfc);
    SDL_FreeSurface(sfc);
}

void BUTTON::b_step(int _x, int _y, int _mx, int _my, bool keyState, int _w, int _h){

    bRect[1].x = _x;
    bRect[1].y = _y;
    bRect[1].w = _w;
    bRect[1].h = _h;

    if(!altern){
        if(_mx > bRect[1].x && _my > bRect[1].y && _mx < bRect[1].x+bRect[1].w && _my < bRect[1].y+bRect[1].h && keyState){



                bState = true;

            }else {
                bState = false;
        }
    }else{

        if(_mx > bRect[1].x && _my > bRect[1].y && _mx < bRect[1].x+bRect[1].w && _my < bRect[1].y+bRect[1].h && keyState && !aux){


            if(!bState_alt){
                bState = true;
            }else {
                bState = false;
            }

            aux = true;

        }else if( _mx > bRect[1].x && _my > bRect[1].y && _mx < bRect[1].x+bRect[1].w && _my < bRect[1].y+bRect[1].h && !keyState && aux){
            if(bState_alt){
                bState_alt= false;
            }else{
                bState_alt = true;
            }
            aux = false;
        }

    }


    if(bState){

        bRect[0].x = bRect[1].w;
    }else{
        bRect[0].x = 0;
    }
}

void BUTTON::b_draw(SDL_Renderer *ren){


    //SDL_SetRenderDrawColor(ren, r, g, b, 255);
    //SDL_RenderDrawRect(ren, &bRect);



    SDL_RenderCopy(ren, tex, &bRect[0], &bRect[1]);



}


bool BUTTON::b_getState(){

    return bState;
}

void BUTTON::b_changeState(bool _state){
    if(bState != _state){
        if(!altern){
            bState = _state;
        }else{

            bState = _state;
            bState_alt = !_state;
            aux = !_state;

        }
    }

}

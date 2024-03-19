#include "editor.h"


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "tool.h"

using namespace std;
/*
EDITOR::EDITOR(){


}

void EDITOR::e_thread(){

}
*/

void mainThread(){

    int x, y;
    int w_screen;
    int h_screen;
    float xcam, ycam;
    bool bDir[4];

    bDir[0] = false;
    bDir[1] = false;
    bDir[2] = false;
    bDir[3] = false;

    int camSpeed = 5;
    bool run;

    SDL_Renderer* ren;
    SDL_Window* win;
    SDL_Event e;

    string fname = "Sin_Nombre";

    w_screen = 1280;
    h_screen = 720;
    run = true;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        cout << "ERROR: " << SDL_GetError() << endl;
    }
    IMG_Init(IMG_INIT_PNG);

    win = SDL_CreateWindow("2D EDITOR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_screen, h_screen, SDL_WINDOW_RESIZABLE);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);


    TOOL t1(128, 128, ren, 10, 1, 10, 100, 4);

    float deltaTime = 0;
    int currentFrame = 0, lastFrame = 0;


    currentFrame = SDL_GetTicks();
    while(run){

        lastFrame = currentFrame;
        currentFrame = SDL_GetTicks();
        deltaTime = (float) (currentFrame - lastFrame / 1000);

        //Keyboard!!
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                run = false;
            }

            if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){

                case SDLK_ESCAPE:
                    run = false;
                    break;

                case SDLK_a:
                    t1.tl_newMap();
                    t1.tl_newStageFile();
                    t1.tl_newTileFile();
                    t1.tl_newActionFile();
                    t1.tl_newEntityFile();
                    break;

                case SDLK_z:
                    t1.tl_saveMap(fname);
                    t1.tl_saveStageFile(fname);
                    t1.tl_saveTileFile(fname);
                    t1.tl_saveActionFile(fname);
                    t1.tl_saveEntityFile(fname);
                    break;

                case SDLK_x:
                    t1.tl_loadMap(fname);
                    t1.tl_loadStageFile(fname);
                    t1.tl_loadTileFile(fname);
                    t1.tl_loadActionFile(fname);
                    t1.tl_loadEntityFile(fname);
                    break;


                case SDLK_p:
                    t1.printMapConsole();
                    break;

                case SDLK_LEFT:
                    bDir[0] = true;
                    break;
                case SDLK_UP:
                    bDir[1] = true;
                    break;
                case SDLK_RIGHT:
                    bDir[2] = true;
                    break;
                case SDLK_DOWN:
                    bDir[3] = true;
                    break;

                default:
                    break;

                }
            }

            if(e.type == SDL_KEYUP){
                switch(e.key.keysym.sym){

                case SDLK_LEFT:
                    bDir[0] = false;
                    break;
                case SDLK_UP:
                    bDir[1] = false;
                    break;
                case SDLK_RIGHT:
                    bDir[2] = false;
                    break;
                case SDLK_DOWN:
                    bDir[3] = false;
                    break;

                default:
                    break;

                }

            }

            t1.tl_keyboard(e);

        }

        //Update
        SDL_GetWindowSize(win, &w_screen, &h_screen);

        if(bDir[0]){ xcam += camSpeed; }
        if(bDir[1]){ ycam += camSpeed; }
        if(bDir[2]){ xcam -= camSpeed; }
        if(bDir[3]){ ycam -= camSpeed; }


        t1.tl_step(xcam, ycam, w_screen, h_screen);


        SDL_SetRenderDrawColor(ren, 100, 100, 100, 1);
        SDL_RenderClear(ren);
        //Dibujo

        t1.tl_draw(ren, w_screen, h_screen, deltaTime);

        SDL_RenderPresent(ren);



    }

}



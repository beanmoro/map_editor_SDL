#include "tool.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>

#include "interface.h"


using namespace std;



namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

TOOL::TOOL(int xMapSize, int yMapSize, SDL_Renderer *ren, int _tFileSize, int _stFileSize, int _acFileSize, int _entFileSize, int _tFrames) :

    tl_button0(0, 0, 32, 32, ren, 0, 32, 32, 32, "data/imgs/action.png", true),
    tl_button1(0, 0, 32, 32, ren, 0, 96, 32, 32, "data/imgs/action.png", true),
    tl_button2(0, 0, 32, 32, ren, 0, 64, 32, 32, "data/imgs/action.png", true),
    tl_button3(0, 0, 32, 32, ren, 0, 128, 32, 32, "data/imgs/action.png", true),

    tl_button_zoom0(0, 0, 16, 16, ren, 0, 224, 16, 16, "data/imgs/action.png", false),
    tl_button_zoom1(0, 0, 16, 16, ren, 0, 240, 16, 16, "data/imgs/action.png", false)

    {
    xTiles = xMapSize;
    yTiles = yMapSize;
    x = 0;
    y = 0;
    sChunk = 32;
    sTile = 32;
    fases = 2; // Default Value (Tile and Actions fase)
    z = _stFileSize;
    stFileSize = z;
    tFileSize = _tFileSize;
    acFileSize = _acFileSize;
    stFileOptions = 3; // Default Value
    tFileOptions = 4; // Default Value
    acFileOptions = 6; //Default Value
    globalTimer = 0;
    oldTime = -1;

    scrnoff_h = 128;
    scrnoff_w = 128;

    entFileSize = _entFileSize;
    entFileOptions = 7;



    mButton[0] = false;
    mButton[1] = false;
    mButton[2] = false;

    zoom = 0.05;

    selectedZ = 0;
    onFase = 0;


    ldActionIMG("data/imgs/action.png", ren);
    ldStripIMG("data/imgs/strip.png", ren);

    sfcTemp = IMG_Load("data/imgs/action.png");
    intfcTex = SDL_CreateTextureFromSurface(ren, sfcTemp);
    SDL_FreeSurface(sfcTemp);


    tileRect[0].x = 0;
    tileRect[0].y = 0;
    tileRect[0].w = sTile;
    tileRect[0].h = sTile;

    actionRect[0].x = 0;
    actionRect[0].y = 0;
    actionRect[0].w = sTile;
    actionRect[0].h = sTile;


    tl_newMap();
    tl_newStageFile();
    tl_newTileFile();
    tl_newActionFile();
    tl_newEntityFile();

    //ldTileFrames();
    uiSel[0].w = sTile*scaleW;
    uiSel[0].h = sTile*scaleH;

    selRect[0] = 0;
    selRect[1] = 0;
    selRect[2] = 0;
    selRect[3] = 0;
/*
    tl_button0(0, 0, 32, 32, 118, 74, 222);
    tl_button1(0, 0, 32, 32, 74, 153, 222);
    tl_button2(0, 0, 32, 32, 74, 202, 222);
    tl_button3(0, 0, 32, 32, 222, 177, 74);
*/
    intfcRect[0].x = 0;
    intfcRect[0].y = 32*5;
    intfcRect[0].w = 32;
    intfcRect[0].h = 32;

}

void TOOL::tl_keyboard(SDL_Event e){

    if(e.type == SDL_KEYDOWN){
        switch(e.key.keysym.sym){
        case SDLK_PLUS:
            scaleW+=zoom;
            scaleH+=zoom;
            break;
        case SDLK_MINUS:
            scaleW-=zoom;
            scaleH-=zoom;
            break;

        case SDLK_m:
            rad +=8;
            break;
        case SDLK_n:
            rad -=8;
            break;


        case SDLK_v:
            selecTile += 1;
            break;
        case SDLK_c:
            selecTile -= 1;
            break;


        case SDLK_h:
            doMultiPainter = true;
            break;

        case SDLK_F1:
            if(!showActionMap){
                showActionMap = true;
            }else{
                showActionMap = false;
            }
            break;
        }

    }

    if(e.type == SDL_KEYUP){
        switch(e.key.keysym.sym){

        default:
            break;
        }
    }




    if(e.type == SDL_MOUSEBUTTONDOWN){
        switch(e.button.button){
        case SDL_BUTTON_LEFT:
            mButton[0] = true;
            break;
        case SDL_BUTTON_MIDDLE:
            mButton[1] = true;
            break;
        case SDL_BUTTON_RIGHT:
            mButton[2] = true;
            break;

        default:
            break;


        }

    }

    if(e.type == SDL_MOUSEBUTTONUP){
        switch(e.button.button){
        case SDL_BUTTON_LEFT:
            mButton[0] = false;
            break;
        case SDL_BUTTON_MIDDLE:
            mButton[1] = false;
            break;
        case SDL_BUTTON_RIGHT:
            mButton[2] = false;
            break;
        default:
            break;


        }

    }
}

void TOOL::tl_step(float _xcam, float _ycam, int _xres, int _yres){

    // sTile/2 = 1 Tile

    SDL_GetMouseState(&mouse_x, &mouse_y);
    mouse_x /= scaleW;
    mouse_y /= scaleH;

    xCam = _xcam;
    yCam = _ycam;


    if(scaleW < zoom){
        scaleW = zoom;
    }
    if(scaleH < zoom){
        scaleH = zoom;
    }

    if(selecTile < 0){
        selecTile =  0;
    }
    if(selecTile > tFileSize){
        selecTile = tFileSize;

    }

    int x_1, y_1, x_2, y_2;

    tl_button0.b_step( (_xres-scrnoff_w+32),32, mouse_x*scaleW , mouse_y*scaleH, mButton[0], 32, 32);
    tl_button1.b_step((_xres-scrnoff_w+32)+32,32, mouse_x*scaleW , mouse_y*scaleH, mButton[0], 32, 32);
    tl_button2.b_step((_xres-scrnoff_w+32),64, mouse_x*scaleW , mouse_y*scaleH, mButton[0], 32, 32);
    tl_button3.b_step((_xres-scrnoff_w+32)+32,64, mouse_x*scaleW , mouse_y*scaleH, mButton[0], 32, 32);

    tl_button_zoom0.b_step((_xres-scrnoff_w+32)+8, 104,  mouse_x*scaleW , mouse_y*scaleH, mButton[0], 16, 16);
    tl_button_zoom1.b_step((_xres-scrnoff_w+32)+40, 104,  mouse_x*scaleW , mouse_y*scaleH, mButton[0], 16, 16);

    if(tl_button0.b_getState() && !drawRad){
        drawRad = true;
        drawRect = false;
        drawPencil = false;
        drawMultiPainter = false;

        tl_button1.b_changeState(drawRect);
        tl_button2.b_changeState(drawPencil);
        tl_button3.b_changeState(drawMultiPainter);

    } else if(tl_button1.b_getState() && !drawRect){
        drawRad = false;
        drawRect = true;
        drawPencil = false;
        drawMultiPainter = false;

        tl_button0.b_changeState(drawRad);
        tl_button2.b_changeState(drawPencil);
        tl_button3.b_changeState(drawMultiPainter);

    } else if(tl_button2.b_getState() && !drawPencil){
        drawRad = false;
        drawRect = false;
        drawPencil = true;
        drawMultiPainter = false;

        tl_button0.b_changeState(drawRad);
        tl_button1.b_changeState(drawRect);
        tl_button3.b_changeState(drawMultiPainter);

    } else if(tl_button3.b_getState() && !drawMultiPainter){
        drawRad = false;
        drawRect = false;
        drawPencil = false;
        drawMultiPainter = true;

        tl_button0.b_changeState(drawRad);
        tl_button1.b_changeState(drawRect);
        tl_button2.b_changeState(drawPencil);

    }else if ( !tl_button0.b_getState() && !tl_button1.b_getState() && !tl_button2.b_getState() && !tl_button3.b_getState() &&
              (drawRad || drawRect || drawPencil || drawMultiPainter) ){
        drawRad = false;
        drawRect = false;
        drawPencil = false;
        drawMultiPainter = false;

    }


    if(tl_button_zoom0.b_getState()){

        scaleH +=zoom/10;
        scaleW +=zoom/10;
    }

    if(tl_button_zoom1.b_getState()){

        scaleH -=zoom/10;
        scaleW -=zoom/10;
    }



    for(int ch = 0; ch < nChunk; ch++){

        if( (mouse_x-xCam > 0 && mouse_y-yCam > 0) && /*(mouse_x-xCam < xChunk*(sTile*sChunk) && mouse_y-yCam < yChunk*(sTile*sChunk)) &&*/ ((mouse_x*scaleW)+xCam < (_xres-scrnoff_h)+xCam && (mouse_y*scaleH)+yCam < (_yres-scrnoff_w)+yCam ) ){

            for(int yy = 0; yy < sChunk; yy++){
                for(int xx = 0; xx < sChunk; xx++){

                    if( (cdChunk[ch].x+(xx*sTile) < mouse_x-xCam) && (cdChunk[ch].y+(yy*sTile) < mouse_y-yCam) && (cdChunk[ch].x+(xx*sTile)+sTile > mouse_x-xCam) && (cdChunk[ch].y+(yy*sTile)+sTile > mouse_y-yCam) ){


                        mTileX = cdChunk[ch].x+(xx*sTile);
                        mTileY = cdChunk[ch].y+(yy*sTile);

                        //HERRAMIENTA: LAPIZ
                        if(mButton[0] && drawPencil){

                            Zone[selectedZ][onFase][ch][yy][xx] = selecTile;

                        }
                        //HERRAMIENTA: MultiLapiz
                        if(mButton[0] && drawMultiPainter){

                            if(!multiPainter[ch][yy][xx]){
                                multiPainter[ch][yy][xx] = true;
                            }

                        }else if(mButton[2] && drawMultiPainter){

                            if(multiPainter[ch][yy][xx]){
                                multiPainter[ch][yy][xx] = false;
                            }

                        }

                    }

                    //HERRAMIENTA: PINCEL
                    if( distPoints(mouse_x-xCam, mouse_y-yCam, cdChunk[ch].x+(xx*sTile)+(sTile/2), cdChunk[ch].y+(yy*sTile)+(sTile/2), rad ) && drawOnMap == true && drawRad == true ){

                        if(mButton[0]){

                            Zone[selectedZ][onFase][ch][yy][xx] = selecTile;

                        }
                    }

                    //HERRAMIENTA: RECTANGULO
                    if(drawRect){

                        if(  ( (cdChunk[ch].x+(xx*sTile) < mouse_x-xCam) && (cdChunk[ch].y+(yy*sTile) < mouse_y-yCam) && (cdChunk[ch].x+(xx*sTile)+sTile > mouse_x-xCam) && (cdChunk[ch].y+(yy*sTile)+sTile > mouse_y-yCam) ) && mButton[0] == true && rectBool == false){
                            selRect[0] = cdChunk[ch].x+(xx*sTile);
                            selRect[1] = cdChunk[ch].y+(yy*sTile);
                            rectBool = true;


                        }

                        if(  ( (cdChunk[ch].x+(xx*sTile) < mouse_x-xCam) && (cdChunk[ch].y+(yy*sTile) < mouse_y-yCam) && (cdChunk[ch].x+(xx*sTile)+sTile > mouse_x-xCam) && (cdChunk[ch].y+(yy*sTile)+sTile > mouse_y-yCam) ) && mButton[0] == false && rectBool == true){

                            selRect[2] = cdChunk[ch].x+(xx*sTile);
                            selRect[3] = cdChunk[ch].y+(yy*sTile);

                            rectBool = false;
                            drawing = true;

                        }

                        if(
                            (
                            //Diagonal a Arriba Derecha
                            ((cdChunk[ch].x+(xx*sTile) >= selRect[0]) && (cdChunk[ch].y+(yy*sTile) >= selRect[1]) &&
                            (cdChunk[ch].x+(xx*sTile)+sTile <= selRect[2]) && (cdChunk[ch].y+(yy*sTile)+sTile <= selRect[3]))

                            //Diagonal a Arriba Izquierda
                             || ((cdChunk[ch].x+(xx*sTile)+sTile <= selRect[0]) && (cdChunk[ch].y+(yy*sTile) >= selRect[1]) &&
                            (cdChunk[ch].x+(xx*sTile) >= selRect[2]) && (cdChunk[ch].y+(yy*sTile)+sTile <= selRect[3]))

                            //Diagonal a Abajo Derecha
                             || ((cdChunk[ch].x+(xx*sTile) >= selRect[0]) && (cdChunk[ch].y+(yy*sTile)+sTile <= selRect[1]) &&
                            (cdChunk[ch].x+(xx*sTile)+sTile <= selRect[2]) && (cdChunk[ch].y+(yy*sTile) >= selRect[3]))

                            //Diagonal a Abajo Izquierda
                             || ((cdChunk[ch].x+(xx*sTile)+sTile <= selRect[0]) && (cdChunk[ch].y+(yy*sTile)+sTile <= selRect[1]) &&
                            (cdChunk[ch].x+(xx*sTile) >= selRect[2]) && (cdChunk[ch].y+(yy*sTile) >= selRect[3]))
                            )

                            && rectBool == false && drawing == true ){

                                if(Zone[selectedZ][onFase][ch][yy][xx] ==3 && (cdChunk[ch].x+(xx*sTile)+sTile >= selRect[2]) && (cdChunk[ch].y+(yy*sTile)+sTile >= selRect[3]) ){
                                    drawing = false;
                                }

                                Zone[selectedZ][onFase][ch][yy][xx] = selecTile;

                           }
                    }
                //Identificar ActionTiles
                    int acZone = Zone[selectedZ][1][ch][yy][xx];


                    for(int i = 0; i < acFileSize; i++){
                        int ac_ch = acCoords[i][0];
                        int ac_x = acCoords[i][2];
                        int ac_y = acCoords[i][1];

                        if(acCoords[i][0]!= -1 && acCoords[i][2]!= -1 && acCoords[i][1]!= -1 && Zone[selectedZ][1][ac_ch][ac_y][ac_x] != i){

                            Zone[selectedZ][1][ac_ch][ac_y][ac_x] = i;
                        }
                    }



                    if(acCoords[acZone][0] != ch && acCoords[acZone][2] != yy  && acCoords[acZone][1] != xx
                       && acCoords[acZone][0] != -1 && acCoords[acZone][2] != -1  && acCoords[acZone][1] != -1
                       ){
                        acCoords[acZone][0] = ch;
                        acCoords[acZone][1] = xx;
                        acCoords[acZone][2] = yy;
                    }
                }
            }
        }
    }

    if(doMultiPainter){

        for(int i = 0; i < nChunk; i++){
            for(int j = 0; j < sChunk; j++){
                for(int k = 0; k < sChunk; k++){

                    if(multiPainter[i][j][k]){
                        Zone[selectedZ][onFase][i][j][k] = selecTile;
                    }
                        //cout << i << "," << j << "," << k << "," << " = "<< selec << endl;
                }
            }
        }

        doMultiPainter = false;
    }

}

void TOOL::tl_draw(SDL_Renderer* _ren, int _xres, int _yres, float _delta){


    for(int ch = 0; ch < nChunk; ch++){

        if( ( ( (cdChunk[ch].x+xCam)*scaleW < _xres+32-scrnoff_w) && ( (cdChunk[ch].y+yCam)*scaleH < _yres+32-scrnoff_h) ) || ( ( (cdChunk[ch].x+xCam+cdChunk[ch].w)*scaleW > -32) && ( (cdChunk[ch].y+yCam+cdChunk[ch].h)*scaleH > -32) ) ){

            for(int yy = 0; yy < sChunk; yy++){
                for(int xx = 0; xx < sChunk; xx++){

                    if( (  ((xx*sTile)+cdChunk[ch].x+xCam)*scaleW < _xres+32-scrnoff_w) && ( ((yy*sTile)+cdChunk[ch].y+yCam)*scaleH < _yres+32-scrnoff_h ) && ( ((xx*sTile)+sTile+cdChunk[ch].x+xCam)*scaleW > -32) && ( ((yy*sTile)+sTile+cdChunk[ch].y+yCam)*scaleH > -32)  ){


                            tileRect[1].x = ((xx*sTile)+xCam+cdChunk[ch].x)*scaleW;
                            tileRect[1].y = ((yy*sTile)+yCam+cdChunk[ch].y)*scaleH;
                            tileRect[1].w = sTile*scaleW;
                            tileRect[1].h = sTile*scaleH;


                            int tileDetected = Zone[selectedZ][onFase][ch][yy][xx];

                            if(tileFrames[tileDetected][0] > 0){

                                if( animTimer[tileDetected] > tileFrames[tileDetected][0] ){
                                    animTimer[tileDetected] = 0;


                                }

                                if(oldTime < (int)(_delta/ (1000*tileFrames[tileDetected][1]) ) ){
                                    animTimer[tileDetected]++;
                                    oldTime = (int)(_delta/ (1000*tileFrames[tileDetected][1]) );

                                }

                                tileRect[0].x = animTimer[tileDetected]*sTile;
                                tileRect[0].y = tileDetected*sTile;



                            }else{

                                tileRect[0].x = 0;
                                tileRect[0].y = tileDetected*sTile;

                            }



                            SDL_RenderCopy(_ren, stripIMG, &tileRect[0], &tileRect[1]);


                            if(showActionMap){
                                int acZone = Zone[selectedZ][1][ch][yy][xx];
                                if(acZone != 0){
                                    if(acTrigger[acZone] == 0 ){
                                        actionRect[0].x = 0;

                                    }else{
                                        actionRect[0].x = 32;


                                    }

                                    SDL_RenderCopy(_ren, actionIMG, &actionRect[0], &tileRect[1]);

                                }

                            }

                            if(drawPencil){

                                uiSel[0].x = (mTileX+xCam)*scaleW;
                                uiSel[0].y = (mTileY+yCam)*scaleH;
                                uiSel[0].w = sTile*scaleW;
                                uiSel[0].h = sTile*scaleH;


                                SDL_SetRenderDrawColor(_ren, 255, 255, 255, 255);
                                SDL_RenderDrawRect(_ren, &uiSel[0]);


                            }

                            if(drawMultiPainter){

                                    if(multiPainter[ch][yy][xx] == true){
                                        uiSel[0].x = ((xx*sTile)+cdChunk[ch].x+xCam)*scaleW;
                                        uiSel[0].y = ((yy*sTile)+cdChunk[ch].y+yCam)*scaleH;
                                        uiSel[0].w = sTile*scaleW;
                                        uiSel[0].h = sTile*scaleH;

                                        SDL_SetRenderDrawColor(_ren, 255, 255, 255, 255);
                                        SDL_RenderDrawRect(_ren, &uiSel[0]);
                                    }
                            }


                            if( distPoints(mouse_x-xCam, mouse_y-yCam, cdChunk[ch].x+(xx*sTile)+(sTile/2), cdChunk[ch].y+(yy*sTile)+(sTile/2), rad ) && drawOnMap == true && drawRad == true){

                                uiSel[0].x = ((xx*sTile)+cdChunk[ch].x+xCam)*scaleW;
                                uiSel[0].y = ((yy*sTile)+cdChunk[ch].y+yCam)*scaleH;
                                uiSel[0].w = sTile*scaleW;
                                uiSel[0].h = sTile*scaleH;

                                SDL_SetRenderDrawColor(_ren, 255, 255, 255, 255);
                                SDL_RenderDrawRect(_ren, &uiSel[0]);

                            }


                            if(drawRect && rectBool == true){



                                uiRect.x = (selRect[0]+xCam)*scaleW;
                                uiRect.y = (selRect[1]+yCam)*scaleW;
                                uiRect.w = ((mTileX)-(selRect[0]) )*scaleW;
                                uiRect.h = ((mTileY)-(selRect[1]) )*scaleH;

                                SDL_SetRenderDrawColor(_ren, 255, 255, 255, 255);
                                SDL_RenderDrawRect(_ren, &uiRect);



                            }

                    }
                }
            }
        }
    }




    intfcRect[1].x = _xres-scrnoff_w;
    intfcRect[1].y = 0;
    intfcRect[1].w = _xres;
    intfcRect[1].h = _yres;

    intfcRect[2].x = 0;
    intfcRect[2].y = _yres-scrnoff_h;
    intfcRect[2].w = _xres-scrnoff_w;
    intfcRect[2].h = _yres;

    SDL_RenderCopy(_ren, intfcTex, &intfcRect[0], &intfcRect[1]);
    SDL_RenderCopy(_ren, intfcTex, &intfcRect[0], &intfcRect[2]);

    tl_button0.b_draw(_ren);
    tl_button1.b_draw(_ren);
    tl_button2.b_draw(_ren);
    tl_button3.b_draw(_ren);

    tl_button_zoom0.b_draw(_ren);
    tl_button_zoom1.b_draw(_ren);





}


void TOOL::ldStripIMG(string filename, SDL_Renderer *_ren){
    /*
    SDL_Surface* temp;
    temp = IMG_Load(filename.c_str());
    stripIMG = SDL_CreateTextureFromSurface(_ren, temp);
    SDL_FreeSurface(temp);
    */
    stripIMG = SDL_CreateTextureFromSurface(_ren, IMG_Load(filename.c_str()));

}

void TOOL::ldActionIMG(string filename, SDL_Renderer *_ren){

    SDL_Surface* temp;
    temp = IMG_Load(filename.c_str());
    actionIMG = SDL_CreateTextureFromSurface(_ren, temp);
    SDL_FreeSurface(temp);


}



void TOOL::tl_newMap(){

    //Consultar dimensiones del mapa;

    crArray();
    cfChunks();

    for(int zz = 0; zz < stFileSize; zz++){
        for(int f = 0; f < fases; f++){
            for(int ch = 0; ch < nChunk; ch++){
                for(int yt = 0; yt < sChunk; yt++){
                    for(int xt = 0; xt < sChunk; xt++){

                        Zone[zz][f][ch][yt][xt] = 0;
                        multiPainter[ch][yt][xt] = false;

                    }
                }
            }
        }
    }
    cout << "Mapa Creado!" << endl;
}

void TOOL::tl_saveMap(string filename){

    string fname;
    for(int r = 0; r < stFileSize; r++){
            for(int t = 0; t < fases; t++){
                string n_Stage = patch::to_string(r);
                string n_Fase = patch::to_string(t);
                fname = filename+"_"+n_Stage +"_"+n_Fase;

                ofstream mapFile;
                mapFile.open(fname.c_str(), ios::out);

                if(mapFile.fail()){

                    cout << "ERROR AL GUARDAR EL ARCHIVO MAPA." << endl;

                }else{

                    int aChunk = 0;
                    // Z, Fases, nChunks, Tiles
                    for(int yc = 0; yc < yChunk; yc++){
                        for(int yt = 0; yt < sChunk; yt++){
                                aChunk = yc*xChunk;
                            for(int xc = 0; xc < xChunk; xc++){
                                for(int xt = 0; xt < sChunk; xt++){

                                    mapFile << Zone[r][t][aChunk][yt][xt] << " ";

                                }
                            aChunk++;
                            }
                            mapFile << endl;
                        }
                    }
                }
        cout << fname << " guardado..." << endl;
        mapFile.close();
        }
    }

    cout << "MAPA GUARDADO!" << endl;
}

void TOOL::tl_loadMap(string filename){

    string fname;
    for(int r = 0; r < stFileSize; r++){
        for(int t = 0; t < fases; t++){

            string n_Stage = patch::to_string(r);
            string n_Fase = patch::to_string(t);
            fname = filename+"_"+n_Stage +"_"+n_Fase;

            ifstream mapFile;
            mapFile.open(fname.c_str(), ios::in);

            if(mapFile.fail()){

                cout << "ERROR AL CARGAR EL ARCHIVO" << endl;

            }else{

                rsArray(fname);
                crArray();
                cfChunks();

                int aChunk = 0;
                for(int yc = 0; yc < yChunk; yc++){
                    for(int yt = 0; yt < sChunk; yt++){
                            aChunk = yc*xChunk;
                        for(int xc = 0; xc < xChunk; xc++){
                            for(int xt = 0; xt < sChunk; xt++){

                                mapFile >> Zone[r][t][aChunk][yt][xt];

                            }
                            aChunk++;
                        }
                    }
                }
            }
            cout << fname << " cargado..." << endl;
            mapFile.close();
        }
    }
    cout << "MAPA CARGADO!" << endl;
}

void TOOL::tl_newStageFile(){

    crSTFile();
    for(int r = 0; r < stFileSize; r++){
        //for(int i = 0; i < stFileOptions; i++){

            stFile[r][0] = "Empty_Level_Name";
            stFile[r][1] = "Empty_Level_Type";
            stFile[r][2] = "Empty_Author_Name";
        //}
    }
}


void TOOL::tl_saveStageFile(string filename){

    ofstream stageFile;
    filename += "_STAGE_FILE";
    stageFile.open(filename.c_str(), ios::out);

    if(stageFile.fail()){

        cout << "ERROR AL GUARDAR ARCHIVO DE ETAPAS" << endl;

    }else{

        for(int r = 0; r < stFileSize; r++){
            for(int i = 0; i < stFileOptions; i++){

                stageFile << stFile[r][i] << " ";

            }
            stageFile << endl;
        }
    }

    stageFile.close();
}

void TOOL::tl_loadStageFile(string filename){

    ifstream stageFile;
    filename += "_STAGE_FILE";
    stageFile.open(filename.c_str(), ios::in);

    if(stageFile.fail()){

        cout << "ERROR AL CARGAR EL ARCHIVO DE ETAPAS" << endl;

    }else{
        rSTFile(filename);
        crSTFile();
        for(int r = 0; r < stFileSize; r++){
            for(int i = 0; i < stFileOptions; i++){

                stageFile >> stFile[r][i];
            }
        }
    }
    stageFile.close();
}

void TOOL::tl_newTileFile(){

    crTFile();

    for(int r = 0; r < tFileSize; r++){
        //for(int i = 0; i < tFileOptions; i++){

            tFile[r][0]= "Empty_Tile_Name";
            tFile[r][1]= "empty.png";
            tFile[r][2]= "0";
            tFile[r][3] = "1";

        //}
    }
}

void TOOL::tl_saveTileFile(string filename){

    ofstream tileFile;
    filename += "_TILE_FILE";
    tileFile.open(filename.c_str(), ios::out);

    if(tileFile.fail()){

        cout << "ERROR AL GUARDAR ARCHIVO DE TILES..." << endl;

    }else{

        for(int i = 0; i < tFileSize; i++){
            for(int r = 0; r < tFileOptions; r++){

                if(r == 2){
                    tileFile << tileFrames[i][0] << " ";
                }else if(r== 3) {

                    tileFile << tileFrames[i][1] << " ";

                }else{
                    tileFile << tFile[i][r] << " ";
                }
            }
            tileFile << endl;
        }
    }
    tileFile.close();
}

void TOOL::tl_loadTileFile(string filename){

    ifstream tileFile;
    filename += "_TILE_FILE";
    tileFile.open(filename.c_str(), ios::in);

    if(tileFile.fail()){


        cout << "ERROR AL CARGAR ARCHIVO DE TILES..." << endl;

    }else{
        rTFile(filename);
        crTFile();

        for(int i = 0; i < tFileSize; i++){

            for(int r = 0; r < tFileOptions; r++){

                if(r == 2){

                    tileFile >> tileFrames[i][0];



                }else if(r == 3){

                    tileFile >> tileFrames[i][1];


                }else{

                    tileFile >> tFile[i][r];

                }


            }
        }
    }
    tileFile.close();
}

void TOOL::tl_newActionFile(){

    crACFile();
    for(int i = 0; i < acFileSize; i++){
        //for(int j = 0; j < acFileOptions; j++){

            acFile[i][0] = "Empty_Action";
            acFile[i][1] = "Empty_Type";
            acFile[i][2] = "0";
            acFile[i][3] = "0";
            acFile[i][4] = "0";
            acCoords[i][0] = -1;
            acCoords[i][1] = -1;
            acCoords[i][2] = -1;
            acTrigger[i] = false;

        //}
    }
}

void TOOL::tl_saveActionFile(string filename){

    ofstream actionFile;
    filename += "_ACTION_FILE";
    actionFile.open(filename.c_str(), ios::out);

    if(actionFile.fail()){

        cout << "ERROR AL GUARDAR EL ARCHIVO DE ACCIONES..." << endl;

    }else{

        for(int i = 0; i < acFileSize; i++){
            for(int j = 0; j < acFileOptions; j++){

                if(j == 2){
                    actionFile << acCoords[i][0] << " ";
                }else if(j == 3){
                    actionFile << acCoords[i][1] << " ";
                }else if(j == 4){
                    actionFile << acCoords[i][2] << " ";
                }else if(j == 5){
                    actionFile << acTrigger[i] << " ";
                }else{
                    actionFile << acFile[i][j] << " ";

                }

            }
            actionFile << endl;
        }
    }
    actionFile.close();
}

void TOOL::tl_loadActionFile(string filename){

    ifstream actionFile;
    filename += "_ACTION_FILE";
    actionFile.open(filename.c_str(), ios::in);

    if(actionFile.fail()){


        cout << "ERROR AL CARGAR EL ARCHIVO DE ACCIONES..." << endl;
    }else{

        rACFile(filename);
        crACFile();
        for(int i = 0; i < acFileSize; i++){
            for(int j = 0; j < acFileOptions; j++){

                if(j == 2){
                    actionFile >> acCoords[i][0];
                }else if(j == 3){
                    actionFile >> acCoords[i][1];
                }else if(j == 4){
                    actionFile >> acCoords[i][2];
                }else if(j == 5){
                    actionFile >> acTrigger[i];
                }else{
                    actionFile >> acFile[i][j];

                }

            }
        }

    }
    actionFile.close();
}


void TOOL::tl_newEntityFile(){

    crENTFile();
    for(int i = 0; i < entFileSize; i++){

        entDB[i][0] = 0; //ID ENTITY
        entDB[i][1] = 0; // TYPE
        entDB[i][2] = 0; // STATE
        entDB[i][3] = 0; // CH
        entDB[i][4] = 0; // X
        entDB[i][5] = 0; // Y
        entDB[i][6] = 0; // Z
    }



}

void TOOL::tl_saveEntityFile(string filename){

    ofstream entityFile;
    filename+="_ENTITY_FILE";
    entityFile.open(filename.c_str(), ios::out);

    if(entityFile.fail()){

        cout << "ERROR AL GUARDAR ARCHIVO DE ENTIDADES..." << endl;
    }else{

        for(int i = 0; i < entFileSize; i++){
            for(int j = 0; j < entFileOptions; j++){

                    entityFile << entDB[i][j] << " ";

            }
            entityFile << endl;
        }

    }
    entityFile.close();
}

void TOOL::tl_loadEntityFile(string filename){

    ifstream entityFile;
    filename += "_ENTITY_FILE";
    entityFile.open(filename.c_str(), ios::in);

    if(entityFile.fail()){

        cout << "ERROR AL CARGAR ARCHIVO DE ENTIDADES..." << endl;

    }else{
        rENTFile(filename);
        crENTFile();
        for(int i = 0; i < entFileSize; i++){
            for(int j = 0; j < entFileOptions; j++){

                entityFile >> entDB[i][j];

            }
        }
    }
    entityFile.close();

}

void TOOL::crENTFile(){

    entDB = new int*[entFileSize];
    for(int i = 0; i < entFileSize; i++){
        entDB[i] = new int[entFileOptions];

    }



}

void TOOL::rENTFile(string filename){

    string line;
    string u;
    ifstream entityFile;
    entityFile.open(filename.c_str(), ios::in);
    int x_size = 0;
    int y_size = 0;

    if(entityFile.fail()){

        cout << "ERROR AL CARGAR EL ARCHIVO DE ENTIDADES EN EL PROCESO DE REESCALADO... " << endl;

    }else{

        while(getline(entityFile, line)){
            istringstream _stream(line);
            x_size = 0;
            while(_stream >> u){
                x_size++;
            }
            y_size++;
        }

        entFileSize = y_size;
        entFileOptions = x_size;

    }
    entityFile.close();

}



void TOOL::rsArray(string filename){

    string line;
    int u;
    ifstream mapFile;
    mapFile.open(filename.c_str(), ios::in);
    int xcounter = 0;
    int ycounter = 0;

    if(mapFile.fail()){

        cout << "ERROR AL RECONFIGURAR EL ARREGLO!" << endl;

    } else {

        while(getline(mapFile, line)){
            istringstream _stream(line);
            xcounter = 0;
            while(_stream >> u){
                xcounter++;
            }
            ycounter++;
        }

        xTiles = xcounter;
        yTiles = ycounter;

    }

    mapFile.close();
}

void TOOL::rSTFile(string filename){

    string line;
    string u;
    ifstream stageFile;
    stageFile.open(filename.c_str(), ios::in);
    int y_size = 0;
    int x_size = 0;

    if(stageFile.fail()){

        cout << "ERROR AL TRATAR DE ENCONTRAR ARCHIVO DE ETAPAS..." << endl;

    } else {
        while(getline(stageFile, line)){
            istringstream _stream(line);
            x_size = 0;
            while(_stream >> u){
                x_size++;
            }
            y_size++;
        }

        stFileSize = y_size;
        stFileOptions = x_size;
    }

}

void TOOL::crSTFile(){

    stFile = new string*[stFileSize];
    for(int i = 0; i < stFileSize; i++){
        stFile[i] = new string[stFileOptions];
    }

}

void TOOL::rTFile(string filename){

    string line;
    string u;
    ifstream tileFile;
    tileFile.open(filename.c_str(), ios::in);
    int x_size = 0;
    int y_size = 0;

    if(tileFile.fail()){


        cout << "ERROR AL CARGAR EL ARCHIVO" << endl;

    }else{
        while(getline(tileFile, line)){
            istringstream _stream(line);
            x_size = 0;
            while(_stream >> u){
                x_size++;

            }

            y_size++;
        }

        tFileSize = y_size;
        tFileOptions = x_size;
    }
    tileFile.close();
}

void TOOL::crTFile(){

    tFile = new string*[tFileSize];
    tileFrames = new float*[tFileSize];
    animTimer = new int[tFileSize];
    for(int i = 0; i < tFileSize; i++){
        tFile[i] = new string[tFileOptions];
        tileFrames[i] = new float [2];
        animTimer[i] = 0;
        tileFrames[i][0] = 0;
        tileFrames[i][1] = 1;


    }


}

void TOOL::crACFile(){

    acFile = new string*[acFileSize];
    acCoords = new int*[acFileSize];
    acTrigger = new bool[acFileSize];
    for(int i = 0; i < acFileSize; i++){
        acFile[i] = new string[acFileOptions];
        acCoords[i] = new int[2];

    }
}

void TOOL::rACFile(string filename){

    string line;
    string u;
    ifstream actionFile;
    actionFile.open(filename.c_str(), ios::in);
    int x_size = 0;
    int y_size = 0;

    if(actionFile.fail()){

        cout << "ERROR AL CARGAR EL ARCHIVO DE ACCIONES EN EL PROCESO DE REESCALADO... " << endl;

    }else{

        while(getline(actionFile, line)){
            istringstream _stream(line);
            x_size = 0;
            while(_stream >> u){
                x_size++;
            }
            y_size++;
        }

        acFileSize = y_size;
        acFileOptions = x_size;

    }
    actionFile.close();
}

void TOOL::crArray(){

    acChunks();
    if( (z != oldZ) || (fases != oldFases) || (nChunk != oldNChunk) || (sChunk != oldSChunk) ){
        multiPainter = new bool**[nChunk];
        Zone = new int****[z];
        for(int i = 0; i < z; i++){
            Zone[i] = new int***[fases];
            for(int j = 0; j < fases; j++){
                Zone[i][j] = new int**[nChunk];
                for(int h = 0; h < nChunk; h++){
                    Zone[i][j][h] = new int *[sChunk];
                    multiPainter[h] = new bool*[sChunk];
                    for(int k = 0; k < sChunk; k++){
                        Zone[i][j][h][k] = new int[sChunk];
                        multiPainter[h][k] = new bool[sChunk];

                    }
                }
            }
        }
        oldZ = z;
        oldFases = fases;
        oldNChunk = nChunk;
        oldSChunk = sChunk;

    }else{

        cout << "Recrear el arreglo del mapa no fue necesario!" << endl;

    }


}



void TOOL::cfChunks(){

    cdChunk = new SDL_Rect[nChunk];
    int t_var = 0;
    int preSize = sTile * sChunk;

    for(int i = 0; i < yChunk; i++){
        t_var = i * xChunk;
        for(int j = 0; j < xChunk; j++){
            cdChunk[t_var].x = preSize*j;
            cdChunk[t_var].y = preSize*i;
            cdChunk[t_var].w = preSize;
            cdChunk[t_var].h = preSize;

            t_var++;
        }
    }
}

void TOOL::acChunks(){

    xChunk = xTiles/sChunk;
    yChunk = yTiles/sChunk;
    nChunk = xChunk*yChunk;
}


void TOOL::printMapConsole(){

    int aChunk = 0;

    for(int yc = 0; yc < yChunk; yc++){
        for(int yt = 0; yt < sChunk; yt++){
            aChunk = yc*xChunk;
            for(int xc = 0; xc < xChunk; xc++){
                for(int xt = 0; xt < sChunk; xt++){

                        cout << Zone[selectedZ][onFase][aChunk][yt][xt] << " ";
                }
                aChunk++;
            }
            cout << endl;
        }
    }



}


bool TOOL::distPoints(int x1, int y1, int x2, int y2, int r){



    return ((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)) < r*r;

}


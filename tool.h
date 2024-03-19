#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>

#include "interface.h"

using namespace std;


#ifndef TOOL_H_INCLUDED
#define TOOL_H_INCLUDED

class TOOL{

    int x, y, z, fases;
    int sChunk, nChunk, xChunk, yChunk, xTiles, yTiles, sTile, stFileSize, stFileOptions, tFileSize, tFileOptions, acFileSize, acFileOptions, tTotalFrames;
    int selectedZ, onFase;
    int wText, hText;
    SDL_Rect uiSel[2];
    float xCam, yCam;
    int globalTimer, oldTime; //Manual Timers
    int oldZ, oldFases, oldNChunk, oldSChunk;
    int *animTimer;
    float **tileFrames;
    int***** Zone; // Map Data File
    string** stFile; // Stage Data File
    string** tFile; //  Tile Data File

    int scrnoff_w,  scrnoff_h;

    SDL_Texture* intfcTex;


    //Action File Vars
    string** acFile;
    int** acCoords;
    bool* acTrigger;

    //ALPHA! Entities File vars
    int** entDB;
    int entFileSize, entFileOptions;



    SDL_Texture *actionIMG = NULL;
    SDL_Texture *stripIMG;

    SDL_Surface *sfcTemp;
    SDL_Rect tileRect[2];
    SDL_Rect actionRect[2];
    SDL_Rect uiRect;
    bool mButton[3];
    bool drawOnMap = true;
    bool drawOnAction = false;
    bool showActionMap = true;
    float scaleW = 1, scaleH = 1;
    float zoom;
    int mouse_x = 0, mouse_y = 0;
    int rad = 64;

    SDL_Rect intfcRect[4];



    //Herramientas
    bool drawPencil = false;
    bool drawRad = false;
    bool drawRect = false;
    bool drawMultiPainter = true;

    bool rectBool = false;
    bool drawing = false;
    bool firstDraw = false;

    bool doMultiPainter = false;
    int mTileX, mTileY;

    int selecTile = 0;

    bool*** multiPainter;

    int selRect[4];

    SDL_Rect* cdChunk;
    void rsArray(string filename); //Resize Array
    void crArray(); // Create Array
    void cfChunks(); // ConFig Chunks
    void acChunks(); //Assign Config Chunks
    void crSTFile(); // Create Stage File
    void rSTFile(string filename); // Resize Stage File
    void crTFile(); // Create Tile File
    void rTFile(string filename); // Resize Tile File
    void ldTileAnim(float a, float b, int tileDetected, int timer); // Load Texture DataBase
    void crACFile(); // Create Action File
    void rACFile(string filename); // Resize Action File
    void ldStripIMG(string filename, SDL_Renderer *_ren);
    void ldActionIMG(string filename, SDL_Renderer *_ren);
    void ldTileFrames();

    void crENTFile();
    void rENTFile(string filename);


    bool distPoints(int x1, int y1, int x2, int y2, int r);

    BUTTON tl_button0;
    BUTTON tl_button1;
    BUTTON tl_button2;
    BUTTON tl_button3;

    BUTTON tl_button_zoom0;
    BUTTON tl_button_zoom1;



public:
    TOOL(int xMapSize, int yMapSize, SDL_Renderer* ren, int _tFileSize, int _stFileSize,int  _acFileSize, int _entFileSize, int _tFrames);
    void tl_newMap();
    void tl_saveMap(string filename);
    void tl_loadMap(string filename);
    void tl_newStageFile();
    void tl_saveStageFile(string filename);
    void tl_loadStageFile(string filename);
    void tl_newTileFile();
    void tl_saveTileFile(string filename);
    void tl_loadTileFile(string filename);
    void tl_newActionFile();
    void tl_saveActionFile(string filename);
    void tl_loadActionFile(string filename);

    void tl_newEntityFile();
    void tl_saveEntityFile(string filename);
    void tl_loadEntityFile(string filename);


    void tl_keyboard(SDL_Event e);
    void tl_step(float _xcam, float _ycam, int _xres, int _yres);
    void tl_draw(SDL_Renderer* _ren, int _xres, int _yres, float _delta);

    //ERROR DETECTION
    void printMapConsole();



};


#endif // TOOL_H_INCLUDED

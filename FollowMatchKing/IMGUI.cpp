//
//  IMGUI.cpp
//  FollowMatchKing
//
//  Created by guanghui on 6/21/17.
//  Copyright © 2017 guanghui. All rights reserved.
//

#include <SDL2/SDL.h>
#include "Utils.h"
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#include <iostream>

// If you're going to render widgets to the same
// UI from different source files, you can avoid
// ID collisions by defining IMGUI_SRC_ID before
// this define block:
#ifdef IMGUI_SRC_ID
#define GEN_ID ((IMGUI_SRC_ID) + (__LINE__))
#else
#define GEN_ID (__LINE__)
#endif

using namespace std;

const int SCREEN_WIDTH = 755;
const int SCREEN_HEIGHT = 600;

struct UIState
{
    int mousex;
    int mousey;
    int mousedown;
    
    int hotitem;
    int activeitem;
}
uistate = {0,0,0,0,0};


SDL_Renderer* _renderer;
SDL_Window* _window;

void drawrect( int x, int y, int w, int h, Uint32 color)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    
    // Set render color to blue ( rect will be rendered in this color )
    Uint8 r = (color & 0xff000000) >> 24;
    Uint8 g = (color & 0x00ff0000) >> 16;
    Uint8 b = (color & 0x0000ff00) >> 8;
    Uint8 a = (color & 0x000000ff);
    SDL_SetRenderDrawColor( _renderer, r, g, b, a );
    
    // Render rect
    SDL_RenderFillRect( _renderer, &rect );
}

// Check whether current mouse position is within a rectangle
int regionhit(int x, int y, int w, int h)
{
    if (uistate.mousex < x ||
        uistate.mousey < y ||
        uistate.mousex >= x + w ||
        uistate.mousey >= y + h)
        return 0;
    return 1;
}

// Simple button IMGUI widget
int button(int id, int x, int y)
{
    // Check whether the button should be hot
    if (regionhit(x, y, 64, 48))
    {
        uistate.hotitem = id;
        if (uistate.activeitem == 0 && uistate.mousedown)
            uistate.activeitem = id;
    }
    
    // Render button
    drawrect(x+8, y+8, 64, 48, 0);
    if (uistate.hotitem == id)
    {
        if (uistate.activeitem == id)
        {
            // Button is both 'hot' and 'active'
            drawrect(x+2, y+2, 64, 48, 0xffffff);
        }
        else
        {
            // Button is merely 'hot'
            drawrect(x, y, 64, 48, 0xffffff);
        }
    }
    else
    {
        // button is not hot, but it may be active
        drawrect(x, y, 64, 48, 0xaaaaaa);
    }
    
    // If button is hot and active, but mouse button is not
    // down, the user must have clicked the button.
    if (uistate.mousedown == 0 &&
        uistate.hotitem == id &&
        uistate.activeitem == id)
        return 1;
    
    return 0;
}

// Prepare for IMGUI code
void imgui_prepare()
{
    uistate.hotitem = 0;
}

// Finish up after IMGUI code
void imgui_finish()
{
    if (uistate.mousedown == 0)
    {
        uistate.activeitem = 0;
    }
    else
    {
        if (uistate.activeitem == 0)
            uistate.activeitem = -1;
    }
}

// Simple scroll bar IMGUI widget
int slider(int id, int x, int y, int max, int &value)
{
    // Calculate mouse cursor's relative y offset
    int ypos = ((256 - 16) * value) / max;
    
    // Check for hotness
    if (regionhit(x+8, y+8, 16, 255))
    {
        uistate.hotitem = id;
        if (uistate.activeitem == 0 && uistate.mousedown)
            uistate.activeitem = id;
    }
    
    // Render the scrollbar
    drawrect(x, y, 32, 256+16, 0x777777);
    
    if (uistate.activeitem == id || uistate.hotitem == id)
    {
        drawrect(x+8, y+8 + ypos, 16, 16, 0xffffff);
    }
    else
    {
        drawrect(x+8, y+8 + ypos, 16, 16, 0xaaaaaa);
    }
    
    // Update widget value
    if (uistate.activeitem == id)
    {
        int mousepos = uistate.mousey - (y + 8);
        if (mousepos < 0) mousepos = 0;
        if (mousepos > 255) mousepos = 255;
        int v = (mousepos * max) / 255;
        if (v != value)
        {
            value = v;
            return 1;
        }
    }
    
    return 0;
}

// Rendering function
void render()
{
    static int bgcolor = 0x770000ff;
    
    // Set render color to red ( background will be rendered in this color )
    SDL_SetRenderDrawColor( _renderer, 255, 0, 0, 255 );
    
    // Clear winow
    SDL_RenderClear(_renderer);
    
    
    // clear screen
    drawrect(0,0, SCREEN_WIDTH, SCREEN_HEIGHT,bgcolor);

    imgui_prepare();
    
    button(GEN_ID,50,50);
    
    button(GEN_ID,150,50);
    
    if (button(GEN_ID,50,150))
        bgcolor = (SDL_GetTicks() * 0xc0cac01a) | 0x77;
    
    if (button(GEN_ID,150,150))
        exit(0);
    
    int slidervalue = bgcolor & 0xff;
    if (slider(GEN_ID, 500, 40, 255, slidervalue))
    {
        bgcolor = (bgcolor & 0xffff00) | slidervalue;
    }
    
    slidervalue = ((bgcolor >> 10) & 0x3f);
    if (slider(GEN_ID, 550, 40, 63, slidervalue))
    {
        bgcolor = (bgcolor & 0xff00ff) | (slidervalue << 10);
    }
    
    slidervalue = ((bgcolor >> 20) & 0xf);
    if (slider(GEN_ID, 600, 40, 15, slidervalue))
    {
        bgcolor = (bgcolor & 0x00ffff) | (slidervalue << 20);
    }
    
    imgui_finish();
    
    SDL_RenderPresent(_renderer);
    
    // don't take all the cpu time
    SDL_Delay(10);

}

int main () {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        king::utils::logSDLError(std::cout, "SDL_Init");
    }
    
    int imageInitFlag = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(imageInitFlag) & imageInitFlag) != imageInitFlag){
        king::utils::logSDLError(std::cout, "IMG_Init");
    }
    
    if (TTF_Init() != 0){
        king::utils::logSDLError(std::cout, "TTF_Init");
    }

    
    _window = SDL_CreateWindow("IMGUI demo", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (_window == nullptr){
        king::utils::logSDLError(std::cout, "CreateWindow");
    }
    
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (_renderer == nullptr){
        king::utils::logSDLError(std::cout, "CreateRenderer");
    }
    
    SDL_Event e;
    bool isRunning = true;
    while (isRunning){
        
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYUP){
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                    break;
                }
            }
            
            if (e.type == SDL_MOUSEMOTION)
            {
                uistate.mousex = e.motion.x;
                uistate.mousey = e.motion.y;
            }
            
            else if( e.type == SDL_MOUSEBUTTONDOWN )
            {
                cout<<"mouse button down\n";
                if(e.button.button == 1) {
                    uistate.mousedown = 1;
                }
                
            }
            else if (e.type == SDL_MOUSEBUTTONUP)
            {
                cout<<"mouse button up\n";
                if (e.button.button == 1) {
                    uistate.mousedown = 0;
                }
            }
        }
        
        render();
    }
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
    

    return 0;
}

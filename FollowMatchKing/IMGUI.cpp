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

using namespace std;

const int SCREEN_WIDTH = 755;
const int SCREEN_HEIGHT = 600;


SDL_Renderer* _renderer;
SDL_Window* _window;

void drawrect( int x, int y, int w, int h, int r, int g, int b)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    
    // Set render color to blue ( rect will be rendered in this color )
    SDL_SetRenderDrawColor( _renderer, r, g, b, 255 );
    
    // Render rect
    SDL_RenderFillRect( _renderer, &rect );
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
            if (e.type == SDL_QUIT){
                isRunning = false;
                break;
            }
            
            if (e.type == SDL_MOUSEMOTION && e.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
            {
                cout<< "left button press move\n";
            }
            
            else if( e.type == SDL_MOUSEBUTTONDOWN )
            {
               cout<<"mouse button down\n";
                
            }
            else if (e.type == SDL_MOUSEBUTTONUP)
            {
              
                cout<<"mouse button up\n";
            }

            
        }
        
        // Set render color to red ( background will be rendered in this color )
        SDL_SetRenderDrawColor( _renderer, 255, 0, 0, 255 );
        
        // Clear winow
        SDL_RenderClear(_renderer);

        
        // test that the fillrect is working
        drawrect(64,48,64,48, 0, 255, 0);
        
//        SDL_UpdateWindowSurface(_window);
        
        SDL_RenderPresent(_renderer);
        
        // don't take all the cpu time
        SDL_Delay(10);
       
    }
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
    

    return 0;
}

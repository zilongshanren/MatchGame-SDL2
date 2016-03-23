//
//  Utils.h
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef Utils_h
#define Utils_h

#include <utility>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

namespace king {
    
    namespace utils {
        
        void logSDLError(std::ostream &os, const std::string &msg);
        
        template<typename T, typename... Args>
        void EasyCleanup(T *t, Args&&... args){
            EasyCleanup(t);
            EasyCleanup(std::forward<Args>(args)...);
        }
        
        //specialized template function implementation should put in .cpp file.
        template<>
        void EasyCleanup<SDL_Window>(SDL_Window *win);
        
        template<>
        void EasyCleanup<SDL_Renderer>(SDL_Renderer *ren);
        
        template<>
        void EasyCleanup<SDL_Texture>(SDL_Texture *tex);
        
        template<>
        void EasyCleanup<SDL_Surface>(SDL_Surface *surf);
        
        template<>
        void EasyCleanup<TTF_Font>(TTF_Font *font);
        
    }
}

#endif /* Utils_h */

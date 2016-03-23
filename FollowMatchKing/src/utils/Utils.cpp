//
//  Utils.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "Utils.h"
#include <iostream>

namespace king {
    namespace utils {
        void logSDLError(std::ostream &os, const std::string &msg){
            os << msg << " error: " << SDL_GetError() << std::endl;
        }
        
        template<>
        void EasyCleanup<SDL_Window>(SDL_Window *win){
            if (!win){
                return;
            }
            SDL_DestroyWindow(win);
        }
        template<>
        void EasyCleanup<SDL_Renderer>(SDL_Renderer *ren){
            if (!ren){
                return;
            }
            SDL_DestroyRenderer(ren);
        }
        template<>
        void EasyCleanup<SDL_Texture>(SDL_Texture *tex){
            if (!tex){
                return;
            }
            SDL_DestroyTexture(tex);
        }
        template<>
        void EasyCleanup<SDL_Surface>(SDL_Surface *surf){
            if (!surf){
                return;
            }
            SDL_FreeSurface(surf);
        }
        
        template<>
        void EasyCleanup<TTF_Font>(TTF_Font *font){
            if (!font){
                return;
            }
            TTF_CloseFont(font);
        }
    }
}

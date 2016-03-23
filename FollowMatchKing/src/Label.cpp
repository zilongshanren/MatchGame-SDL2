//
//  Label.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/9/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "Label.hpp"
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "Utils.h"

static const SDL_Color WHITE = {255,255,255, 255};

namespace king {
    Label::Label(_TTF_Font* font, SDL_Renderer* renderer)
    :_font(font),
    _renderer(renderer)
    {
        
    }
    
    Label::~Label()
    {
        if (_texture) {
            utils::EasyCleanup(_texture);
        }
    }
    
    SDL_Texture* Label::renderText(const std::string &message, SDL_Color color)
    {
        //We need to first render to a surface as that's what TTF_RenderText
        //returns, then load that surface into a texture
        SDL_Surface *surf = TTF_RenderText_Blended(_font, message.c_str(), color);
        if (surf == nullptr){
            TTF_CloseFont(_font);
            king::utils::logSDLError(std::cout, "TTF_RenderText");
            return nullptr;
        }
        
        SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surf);
        if (texture == nullptr){
            king::utils::logSDLError(std::cout, "CreateTexture");
        }
        //Clean up the surface and font
        SDL_FreeSurface(surf);
        return texture;
    }
    
    void Label::setText(const std::string &message)
    {
        _message = message;
        if (_texture) {
            utils::EasyCleanup(_texture);
        }
        this->setTexture(this->renderText(_message, WHITE));
    }
}
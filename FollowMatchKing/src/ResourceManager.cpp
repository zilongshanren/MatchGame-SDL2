//
//  ResourceManager.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "ResourceManager.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "utils/Utils.h"


namespace king {
    
    ResourceManager::ResourceManager(SDL_Renderer* renderer)
    :_renderer(renderer)
    {
        
    }
    
    ResourceManager::~ResourceManager()
    {
        for (auto&& texutre : _textures)
        {
            utils::EasyCleanup(texutre.second);
        }
        
        for (auto&& font : _ttfFonts)
        {
            utils::EasyCleanup(font.second);
        }
        _textures.clear();
        _ttfFonts.clear();
        
        TTF_Quit();
        SDL_Quit();
    }
    
    SDL_Texture* ResourceManager::addImage(const std::string &filename)
    {
        SDL_Texture* texture = nullptr;
        
        auto it = _textures.find(filename);
        if( it != _textures.end())
        {
            texture = it->second;
        }
        
        if (!texture)
        {
            texture = IMG_LoadTexture(_renderer, filename.c_str());
            if (texture == nullptr)
            {
                utils::logSDLError(std::cout, "addImage");
            }
            else
            {
                _textures.insert(std::make_pair(filename, texture));
            }
        }

        return texture;
    }
    
    SDL_Texture* ResourceManager::getImageForKey(const std::string &filename)
    {
        auto it = _textures.find(filename);
        
        if( it != _textures.end())
        {
            return it->second;
        }
        return nullptr;
    }
    
    TTF_Font* ResourceManager::addTTFFont(const std::string &filename, int fontSize)
    {
        TTF_Font* font = nullptr;

        auto it = _ttfFonts.find(std::make_pair(filename, fontSize));
        if (it != _ttfFonts.end())
        {
            font = it->second;
        }
        
        if (!font)
        {
            font = TTF_OpenFont(filename.c_str(), fontSize);
            if (font == nullptr)
            {
                utils::logSDLError(std::cout, "addTTFFont");
            }
            else
            {
                _ttfFonts.insert(std::make_pair(std::make_pair(filename, fontSize), font));
            }
        }
       
        return font;
    }
    
    TTF_Font* ResourceManager::getTTFFontForKey(const std::string &filename, int fontSize)
    {
        auto it = _ttfFonts.find(std::make_pair(filename, fontSize));
        if (it != _ttfFonts.end())
        {
            return it->second;
        }
        return nullptr;
    }
}
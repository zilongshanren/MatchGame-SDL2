//
//  Label.hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/9/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef Label_hpp
#define Label_hpp
#include <string>
#include <memory>

#include "GameObject.hpp"

class _TTF_Font;
class SDL_Renderer;

namespace king {
    
    class Label : public GameObject
    {
    public:
        Label(_TTF_Font* font, SDL_Renderer* renderer);
        ~Label();
        void setText(const std::string& message);

    private:
        SDL_Texture* renderText(const std::string &message, SDL_Color color);
        
        std::string _message;
        _TTF_Font* _font; //weak ref
        SDL_Renderer *_renderer; //weak ref
    };
}

#endif /* Label_hpp */

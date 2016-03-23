//
//  GameObject.hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <SDL2/SDL.h>

#include "Renderable.h"
#include "SimpleMath.h"

namespace king {
    class GameObject : public Renderable
    {
    public:
        GameObject();
        ~GameObject();
        
        void setPosition(math::Vec2 position);
        math::Vec2 getPosition() const;
        void setSize(math::Size size);
        math::Size getSize() const;
        bool isVisible() const;
        void setVisible(bool visible);
        
        void setTexture(SDL_Texture* texture);
        
        virtual void render(SDL_Renderer* renderer) override;
        
        virtual void update(float dt){};
        virtual void handleEvent(SDL_Event event){};
        
        virtual bool hitTest(int x, int y);
    protected:
        SDL_Texture *_texture; //weak ref
    private:
        SDL_Rect _boundingBox;
        bool _visible;
    };
}

#endif /* GameObject_hpp */

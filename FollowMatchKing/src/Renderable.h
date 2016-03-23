//
//  Renderable.h
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef Renderable_h
#define Renderable_h

class SDL_Renderer;

namespace king {
    /**
     *
     */
    class Renderable
    {
    public:
        virtual void render(SDL_Renderer* renderer) = 0;
        
        virtual ~Renderable() {}
    };
}

#endif /* Renderable_h */

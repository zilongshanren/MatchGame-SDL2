//
//  TileSprite.hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef TileSprite_hpp
#define TileSprite_hpp

#include <memory>
#include <vector>

#include "GameObject.hpp"

namespace king {
    class Game;
    class Tile;
    /**
     * Rendering tile and handle tile animation
     */
    class TileSprite : public GameObject
    {
    public:
              
        TileSprite(Game* game, int type);
        ~TileSprite();
                
        void swap(TileSprite* other);
        void invalidSwap(TileSprite* other);
        void setType(int type);
    private:
        Game* _game; //weak ref
        int _type;
    };
}

#endif /* TileSprite_hpp */

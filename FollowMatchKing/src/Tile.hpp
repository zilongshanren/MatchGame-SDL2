//
//  Tile.hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef Tile_hpp
#define Tile_hpp

#include <iostream>

namespace king {
    class TileSprite;
    
    class Tile
    {
    public:
        enum class Type
        {
            Blue = 1,
            Green = 2,
            Purple = 3,
            Red = 4,
            Yellow = 5
        };

        Tile();
        ~Tile();
        
        bool isAdjacent(Tile* tile) const;
        void swap(Tile* tile);
        void invalidSwap(Tile* tile);
        TileSprite* getSprite() const;
        void setSprite(TileSprite* sprite);
        
        friend std::ostream & operator << (std::ostream &os, const Tile & tile);

        int _row;
        int _column;
        Type _type;
        
    private:
        TileSprite* _sprite; //weak ref
    };
}

#endif /* Tile_hpp */

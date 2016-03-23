//
//  Tile.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "Tile.hpp"
#include <cstdlib>

#include "TileSprite.hpp"

namespace king {
    Tile::Tile()
    :_column(0),
    _row(0),
    _type(Type::Blue),
    _sprite(nullptr)
    {
        
    }
    
    Tile::~Tile()
    {
        std::cout<<"~Tile\n";
    }
    
    bool Tile::isAdjacent(king::Tile *tile) const
    {
        //in the same column
        if (abs(_column - tile->_column) == 0)
        {
            if (abs(_row - tile->_row) == 1) {
                return true;
            }
            return false;
        }
        //in the same row
        if (abs(_row - tile->_row) == 0) {
            if (abs(_column -tile->_column) == 1) {
                return true;
            }
            return false;
        }
        
        return false;
    }
    
    void Tile::swap(king::Tile *tile)
    {
        //move sprite position
        _sprite->swap(tile->getSprite());
    }
    
    void Tile::invalidSwap(king::Tile *tile)
    {
        _sprite->invalidSwap(tile->getSprite());
    }
    
    TileSprite* Tile::getSprite() const
    {
        return _sprite;
    }
    
    void Tile::setSprite(king::TileSprite *sprite)
    {
        _sprite = sprite;
    }
    
    std::ostream & operator << (std::ostream &os, const Tile & tile)
    {
        os<<"("<<tile._row <<","<<tile._column<<", "<<(int)tile._type<<"), ";
        return os;
    }

    
    
}
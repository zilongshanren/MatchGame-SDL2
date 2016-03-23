//
//  MachingSequence.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/7/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "Chain.hpp"
#include "Tile.hpp"

namespace king {
    Chain::Chain()
    :_type(Type::Horizontal)
    {
        
    }
    
    Chain::~Chain()
    {
    }
    
    void Chain::setType(king::Chain::Type type)
    {
        _type = type;
    }
    
    void Chain::addTile(Tile* tile)
    {
        _tiles.push_back(tile);
    }
    
    std::vector<Tile*> Chain::getTiles() const
    {
        return _tiles;
    }

}
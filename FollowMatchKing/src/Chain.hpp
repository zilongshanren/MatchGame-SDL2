//
//  MatchingSequence.hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/7/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef Chain_hpp
#define Chain_hpp

#include <vector>
#include <memory>

namespace king {
    class Tile;
    
    class Chain
    {
    public:
        Chain();
        ~Chain();
        enum class Type
        {
            Horizontal,
            Vertical
        };
        
        void addTile(Tile* tile);
        std::vector<Tile*> getTiles() const;
        void setType(Type type);
    private:
        Type _type;
        std::vector<Tile*> _tiles;
    };
}

#endif /* Chain_hpp */

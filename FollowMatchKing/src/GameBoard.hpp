//
//  GameBoard.hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef GameBoard_hpp
#define GameBoard_hpp

#include <vector>
#include <set>

#include "Tile.hpp"
#include "Constants.h"

namespace king {
    class Chain;
    
    class GameBoard
    {
    public:
        GameBoard();
        ~GameBoard();
        
        void fillBoard();
        Tile* getTile(int row, int col);
        
        void swap(Tile* tileA, Tile* tileB);
        
        void invalidSwap(Tile* tileA, Tile* tileB);
                
        void print();
        
        void detectPossibleMoves();
        bool hasValidSequence(int row, int column);
        bool isPossibleMove(std::pair<Tile*, Tile*> move);
        std::set<Chain*> removeMatches();
        void removeTiles(std::set<Chain*> set);
        std::vector<std::vector<Tile*>> fillHoles();
        std::vector<std::vector<Tile*>> addNewTiles();
        
        void removeMatchesTiles();
    private:
        Tile* createTile(int row, int col, int type);
        std::set<Tile*> initTiles();
        std::set<Chain*> detectHorizontalMatches();
        std::set<Chain*> detectVerticalMatches();
        //return a diffrent tile type from the previous one.
        Tile::Type getNextRandomTileType(Tile::Type previousType) const;
        
        Tile *_tiles[kMaxNumberOfRows][kMaxNumberOfColumns];
        
        //we use this set to remove matched tiles.
        std::set<Tile*> _removeTiles;
        
        std::set<std::pair<Tile*, Tile*>> _possileMoves;
    };
}

#endif /* GameBoard_hpp */

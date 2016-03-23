//
//  GameBoard.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//
#include "GameBoard.hpp"
#include <cstdlib>
#include <cassert>
#include <algorithm>

#include "Tile.hpp"
#include "Constants.h"
#include "Chain.hpp"

namespace king {
    
    
    GameBoard::GameBoard()
    {
    }
    
    GameBoard::~GameBoard()
    {
        for (int i = 0; i < kMaxNumberOfRows; ++i) {
            for (int j = 0; j < kMaxNumberOfColumns; ++j) {
                if (_tiles[i][j]) {
                    delete _tiles[i][j];
                    _tiles[i][j] = nullptr;
                }
            }
        }

    }
    
    std::set<Tile*> GameBoard::initTiles()
    {
        std::set<Tile*> set;
        for (int row = 0; row < kMaxNumberOfRows; ++row) {
            for (int column = 0; column < kMaxNumberOfColumns; ++column) {
                
                int tileType;
                do {
                    tileType = rand() % kNumOfRubies + 1;
                }
                while ((row >= 2 &&
                        (int)_tiles[row - 1][column]->_type == tileType &&
                        (int)_tiles[row - 2][column]->_type == tileType)
                       ||
                       (column >= 2 &&
                        (int)_tiles[row][column - 1]->_type == tileType &&
                        (int)_tiles[row][column - 2]->_type == tileType));
                
                auto tile = this->createTile(row, column,tileType);
                
                set.insert(tile);
            }
        }
        return set;
    }
    
    void GameBoard::detectPossibleMoves()
    {
        _possileMoves.clear();
        for (int row = 0; row < kMaxNumberOfRows; ++row) {
            for (int column = 0; column < kMaxNumberOfColumns; ++column) {
                auto tile = _tiles[row][column];
                
                if (tile) {
                    
                    if (column < kMaxNumberOfColumns - 1) {
                        auto otherTile = _tiles[row][column+1];
                        
                        if (otherTile) {
                            //try to swap the two tiles
                            _tiles[row][column] = otherTile;
                            _tiles[row][column+1] = tile;
                            
                            if (this->hasValidSequence(row, column) ||
                                this->hasValidSequence(row, column + 1)) {
                                _possileMoves.insert(std::make_pair(tile, otherTile));
                            }
                            
                            //swap back
                            _tiles[row][column] = tile;
                            _tiles[row][column+1] = otherTile;
                        }
                    }
                    
                    if (row < kMaxNumberOfRows - 1) {
                        auto otherTile = _tiles[row+1][column];
                        
                        if (otherTile) {
                            _tiles[row][column] = otherTile;
                            _tiles[row+1][column] = tile;
                            
                            if (this->hasValidSequence(row, column) ||
                                this->hasValidSequence(row + 1, column)) {
                                _possileMoves.insert(std::make_pair(tile, otherTile));
                            }
                            
                            //swap back
                            _tiles[row][column] = tile;
                            _tiles[row+1][column] = otherTile;
                        }
                    }
                }
            }
        }
    }
    
    std::set<Chain*> GameBoard::removeMatches()
    {
        _removeTiles.clear();
        auto horizontalMatches = this->detectHorizontalMatches();
        auto verticalMatches = this->detectVerticalMatches();
        
        this->removeTiles(horizontalMatches);
        this->removeTiles(verticalMatches);
        
        horizontalMatches.insert(verticalMatches.cbegin(), verticalMatches.cend());
        
        return horizontalMatches;
    }
    
    void GameBoard::removeTiles(std::set<Chain *> set)
    {
        for(auto && chain : set)
        {
            for(auto && tile : chain->getTiles())
            {
                _tiles[tile->_row][tile->_column] = nullptr;
                _removeTiles.insert(tile);
            }
        }
    }
    
    void GameBoard::removeMatchesTiles()
    {
        for(auto&& tile: _removeTiles)
        {
            delete tile;
        }
    }
    
    std::set<Chain*> GameBoard::detectHorizontalMatches()
    {
        std::set<Chain*> set;
        for (int row = 0; row < kMaxNumberOfRows; ++row) {
            for (int column = 0; column < kMaxNumberOfColumns - 2;) {
                if (_tiles[row][column]) {
                    auto matchType = _tiles[row][column]->_type;
                    
                    if (_tiles[row][column+1]->_type == matchType
                        && _tiles[row][column+2]->_type == matchType) {
                        
                        auto chain = new Chain;
                        chain->setType(Chain::Type::Horizontal);
                        do
                        {
                            chain->addTile(_tiles[row][column]);
                            column++;
                        }while (column < kMaxNumberOfColumns
                                && _tiles[row][column]->_type == matchType);
                        
                        set.insert(chain);
                        continue;
                    }
                }
                column++;
            }
        }
        
        return set;
    }
    
    std::set<Chain*> GameBoard::detectVerticalMatches()
    {
        std::set<Chain*> set;
        for (int column = 0; column < kMaxNumberOfColumns; ++column) {
            for (int row = 0; row < kMaxNumberOfRows - 2;) {

                if (_tiles[row][column]) {
                    auto matchType = _tiles[row][column]->_type;
                    
                    if (_tiles[row+1][column]->_type == matchType
                        && _tiles[row+2][column]->_type == matchType) {
                        
                        auto chain = new Chain;
                        chain->setType(Chain::Type::Vertical);
                        
                        do {
                            chain->addTile(_tiles[row][column]);
                            ++row;
                        } while (row < kMaxNumberOfRows
                                 && _tiles[row][column]->_type == matchType);
                        
                        set.insert(chain);
                        continue;
                    }
                }
                row++;
            }
        }
        
        return set;
    }
    
    bool GameBoard::isPossibleMove(std::pair<Tile *, Tile *> move)
    {
        return std::find_if(_possileMoves.begin(), _possileMoves.end(),
                            [=](std::pair<Tile*, Tile*>  pair) -> bool{
            return (pair.first == move.first && pair.second == move.second) ||
                    (pair.first == move.second && pair.second == move.first);
        }) != _possileMoves.end();
    }
    
    void GameBoard::fillBoard()
    {
        do {
            this->initTiles();
            
            this->detectPossibleMoves();
            
        }while (_possileMoves.size() == 0);
      
        for (auto&& item : _possileMoves) {
            std::cout<<*item.first<<","<<*item.second<<std::endl;
        }
    }
    
    Tile::Type GameBoard::getNextRandomTileType(Tile::Type previousType)const
    {
        Tile::Type newType = Tile::Type::Blue;
        
        do {
            newType = (Tile::Type)(rand() % (int)(Tile::Type::Yellow) + 1);
        } while (newType == previousType);
        
        return newType;
    }
    
    std::vector<std::vector<Tile*>> GameBoard::addNewTiles()
    {
        std::vector<std::vector<Tile*>> tilesStack;
        
        Tile::Type tileType = Tile::Type::Blue;
        for (int column = 0; column < kMaxNumberOfColumns; ++column) {
            std::vector<Tile*> stack;
            for (int row = kMaxNumberOfRows - 1; row >=0; --row) {
                if (_tiles[row][column] == nullptr) {
                    //create new tile
                    auto newTiletype = this->getNextRandomTileType(tileType);
                    
                    auto newTile = this->createTile(row, column, (int)newTiletype);
                    stack.push_back(newTile);
                    
                    tileType = newTiletype;
                }
            }
            
            if (stack.size() > 0) {
                tilesStack.push_back(stack);
            }
        }
        
        return tilesStack;
    }
    
    bool GameBoard::hasValidSequence(int row, int column)
    {
        auto tileType = _tiles[row][column]->_type;
        
        auto verticalLength = 1;
        for (int i = row - 1; i >= 0 ; --i)
        {
            if(_tiles[i][column]->_type == tileType)
            {
                ++verticalLength;
            }
            else
            {
                break;
            }
        }
        
        for (int i = row + 1; i < kMaxNumberOfRows; ++i) {
            if(_tiles[i][column]->_type == tileType)
            {
                ++verticalLength;
            }
            else
            {
                break;
            }
        }
        
        if (verticalLength >= 3) return true;
        
        //check for column
        auto horizontalLeght = 1;
        for (int i = column - 1; i >= 0; i--) {
            if (_tiles[row][i]->_type == tileType) {
                horizontalLeght++;
            }
            else
            {
                break;
            }
        }
        
        for (int i = column + 1; i < kMaxNumberOfColumns; ++i) {
            if (_tiles[row][i]->_type == tileType) {
                horizontalLeght++;
            }
            else{
                break;
            }
        }
        
        return horizontalLeght >= 3;
    }
    
    std::vector<std::vector<Tile*>> GameBoard::fillHoles()
    {
        std::vector<std::vector<Tile*>> tileStacks;
        
        for (int column = 0; column < kMaxNumberOfColumns; ++column) {
            std::vector<Tile*> stack;
            
            for (int row = 0; row < kMaxNumberOfRows; ++row) {
                
                if (_tiles[row][column] == nullptr) {
                    
                    for (int lookup = row+1; lookup < kMaxNumberOfRows; ++lookup) {
                        auto tile = _tiles[lookup][column];
                        if (tile != nullptr) {
                            _tiles[row][column] = tile;
                            _tiles[lookup][column] = nullptr;
                            tile->_row = row;
                            
                            stack.push_back(tile);
                            
                            break;
                        }
                    }
                }
            }
            
            if (stack.size() > 0) {
                tileStacks.push_back(stack);
            }
        }
        
        return tileStacks;
    }
    
    Tile* GameBoard::createTile(int row, int col, int type)
    {
        auto tile = new Tile();
        tile->_row = row;
        tile->_column = col;
        tile->_type = (Tile::Type)type;
        
        _tiles[row][col] = tile;
        return tile;
    }
    
    void GameBoard::swap(king::Tile *tileA, king::Tile *tileB)
    {

        int rowA = tileA->_row;
        int colA = tileA->_column;
        int rowB = tileB->_row;
        int colB = tileB->_column;
        
        _tiles[rowA][colA] = tileB;
        tileB->_row = rowA;
        tileB->_column = colA;
    
        _tiles[rowB][colB] = tileA;
        tileA->_row = rowB;
        tileA->_column = colB;
        
        tileA->swap(tileB);
    }
    
    void GameBoard::invalidSwap(king::Tile *tileA, king::Tile *tileB)
    {
        tileA->invalidSwap(tileB);
    }
    
    void GameBoard::print()
    {
        std::cout<<"print board\n";
        for (int i = 0; i < kMaxNumberOfRows; ++i) {
            for (int j = 0; j < kMaxNumberOfColumns; ++j) {
                if (_tiles[i][j]) {
                    std::cout<<*_tiles[i][j];
                }
                else{
                    std::cout<<" xx,";
                }
            }
            std::cout<<std::endl;
        }
        
//        std::cout<<"print possible moves:\n";
//        for (auto&& item : _possileMoves) {
//            std::cout<<*item.first<<","<<*item.second<<std::endl;
//        }
    }
    
    Tile* GameBoard::getTile(int row, int col)
    {
        return _tiles[row][col];
    }
    
    
}
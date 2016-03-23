//
//  GameScene.hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <memory>
#include <vector>

#include "Renderable.h"
#include "GameObject.hpp"
#include "Tile.hpp"
#include "GameBoard.hpp"

class SDL_Texture;

namespace king {
    class Game;
    class TileSprite;
    class Label;
    
    class GameScene : public GameObject
    {
    public:
        
        explicit GameScene(Game* game);
        ~GameScene();
        
        virtual void render(SDL_Renderer* renderer) override;
        virtual void update(float dt) override;
        virtual void handleEvent(SDL_Event event) override;
        
        GameBoard* getGameBoard() const;
        void animateFallingTiles(std::vector<std::vector<Tile*>> tiles);
        
        void handleMatches();
        
        void handleGameState();
    protected:
        void initGameBoard();
        void swapTiles();
        void handleNewTilesAndFalling();
        
        math::Vec2 convertMousePosition(SDL_Event event);
        math::Vec2 convertTileCoordinatePosition(int row, int colum);
        
        Tile* getTileSprite(Tile* tile);
        Tile* getTileSprite(int row, int column);
    private:
        Game* _game; //weak ref
        TileSprite* getAvailableTileSprite() const;
        void initLabels();
        
        Tile* _firstSelectedTile; //weak ref
        Tile* _secondSelectedTile; //weak ref

        std::unique_ptr<GameObject> _background;
        std::unique_ptr<GameBoard> _gameBoard;
        
        //for rendering
        std::vector<std::unique_ptr<TileSprite>> _tileSprites;
        
        bool _isValidClick;
        Tile* _selectedTile; //weak ref
        int _selectRow;
        int _selectColumn;
        
        std::unique_ptr<Label> _timeTitle;
        std::unique_ptr<Label> _timeRemainingLabel;
        std::unique_ptr<Label> _gameOverLabel;
        float _remainingTime;
    };
}

#endif /* GameScene_hpp */

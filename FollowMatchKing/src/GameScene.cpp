//
//  GameScene.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "GameScene.hpp"

#include <assert.h>
#include <sstream>
#include <SDL2/SDL.h>

#include "GameObject.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Constants.h"
#include "SimpleMath.h"
#include "TileSprite.hpp"
#include "ActionManager.hpp"
#include "Action.hpp"
#include "TweenFunction.hpp"
#include "Chain.hpp"
#include "Label.hpp"

namespace king {
    using namespace math;
    
    static const int kTileWidth = 42;
    static const int kTileHeight = 42;
    static const int kBoardStartPositionX = 330;
    static const int kBoardStartPositionY = 400;
    
    GameScene::GameScene(Game *game)
    :_game(game),
    _firstSelectedTile(nullptr),
    _secondSelectedTile(nullptr),
    _isValidClick(false),
    _selectedTile(nullptr),
    _selectRow(-1),
    _selectColumn(-1),
    _remainingTime(60)
    {
        //init background
        _background = std::unique_ptr<GameObject>(new GameObject());
        auto bgTexture = game->getResourceManager()->addImage(kBackgroundImageName);
        _background->setTexture(bgTexture);
        
        _gameBoard = std::unique_ptr<GameBoard>(new GameBoard());
        
        this->initGameBoard();
        this->initLabels();
    }
    
    GameScene::~GameScene()
    {
        
    }
    
    void GameScene::initLabels()
    {
        auto font = _game->getResourceManager()->addTTFFont("arial.ttf", 30);
        _timeTitle = std::unique_ptr<Label>(new Label(font, _game->getRenderer()));
        _timeTitle->setText("Time: ");
        _timeTitle->setPosition(Vec2(10,30));
        
        _timeRemainingLabel = std::unique_ptr<Label>(new Label(font, _game->getRenderer()));
        _timeRemainingLabel->setText(std::to_string((int)_remainingTime));
        _timeRemainingLabel->setPosition(Vec2(100,30));
        
        font = _game->getResourceManager()->addTTFFont("arial.ttf", 60);
        _gameOverLabel = std::unique_ptr<Label>(new Label(font, _game->getRenderer()));
        _gameOverLabel->setText("Game Over!");
        _gameOverLabel->setPosition(Vec2(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 100));
        _gameOverLabel->setVisible(false);
    }
    
    void GameScene::initGameBoard()
    {
        _gameBoard->fillBoard();
        
        float startX = kBoardStartPositionX;
        float startY = kBoardStartPositionY;
        
        float tileWidth = kTileWidth;
        float tileHeight = kTileHeight;
        
        for (int i = 0; i < kMaxNumberOfRows; ++i) {
            for (int j = 0; j < kMaxNumberOfColumns; ++j) {
                auto tile = _gameBoard->getTile(i, j);
                
                auto tileSprite = new TileSprite(_game, (int)tile->_type);
                tile->setSprite(tileSprite);
                
                tileSprite->setPosition(Vec2( startX + j * tileWidth,
                                             startY - i * tileHeight));
                _tileSprites.push_back(std::unique_ptr<TileSprite>(tileSprite));
            }
        }
    }
    
    void GameScene::render(SDL_Renderer *renderer)
    {
        _background->render(renderer);
        
        //render game board
        for(auto && object : _tileSprites)
        {
            object->render(renderer);
        }
        _timeTitle->render(renderer);
        _timeRemainingLabel->render(renderer);
        _gameOverLabel->render(renderer);
    }
    
    void GameScene::handleMatches()
    {
        auto gameBoard = _game->getGameScene()->getGameBoard();
        auto chains = gameBoard->removeMatches();
        
        if (chains.size() == 0) {
            _game->setState(Game::State::Idle);
            return;
        }
        
        //animate the matched chain remove
        for(auto && chain : chains)
        {
            for(auto && tile: chain->getTiles())
            {
                auto sprite = tile->getSprite();
                if (sprite) {
                    sprite->setVisible(false);
                    //TODO: implement fade in action
                    auto action = _game->getActionManager()->addAction(sprite);
                    action->setDuration(0.15);
                    action->start();
                }                
            }
        }
        
        //free matched tiles
        _gameBoard->removeMatchesTiles();
        for (auto && chain : chains){
            delete chain;
        }
        chains.clear();
    }
    
    void GameScene::update(float dt)
    {
        for(auto && object : _tileSprites)
        {
            object->update(dt);
        }
        if (_game->getCurrentState() == Game::State::Idle) {
            _remainingTime -= dt;
            if (_remainingTime <= 0) {
                _gameOverLabel->setVisible(true);
                _remainingTime = 0;
                _game->setState(Game::State::GameOver);
            }
            _timeRemainingLabel->setText(std::to_string((int)_remainingTime));
        }
    }
    
    void GameScene::handleNewTilesAndFalling()
    {
        auto newtiles = _gameBoard->addNewTiles();
        
        for(auto && stack : newtiles)
        {
            int index = 0;
            for(auto&& tile: stack)
            {
                auto sprite = this->getAvailableTileSprite();
                sprite->setType((int)tile->_type);
                sprite->setVisible(true);
                tile->setSprite(sprite);
                sprite->setPosition(this->convertTileCoordinatePosition(tile->_row+1, tile->_column));
                
                float delay = 0.05 + index * 0.15;
                float duration = 0.2;
                
                auto action = _game->getActionManager()->addAction(sprite);
                action->setDestination(this->convertTileCoordinatePosition(tile->_row, tile->_column));
                action->setDuration(duration);
                action->setTweenFunc(std::bind(math::easeOut,std::placeholders::_1, 1));
                action->start();
                action->setDelay(delay);
            }
        }
    }
    
    void GameScene::handleGameState()
    {
        auto state = _game->getCurrentState();
        
        if (state == Game::State::Falling) {
            auto tileStacks = _gameBoard->fillHoles();
            this->animateFallingTiles(tileStacks);
        }
        
        if (state == Game::State::AddNewTiles) {
            this->handleNewTilesAndFalling();
            this->_gameBoard->detectPossibleMoves();
        }
        
        if (state == Game::State::RemoveMatches) {
            this->handleMatches();
        }
    }
    
    Tile* GameScene::getTileSprite(int row, int column)
    {
        auto tile = _gameBoard->getTile(row, column);
        return tile;
    }
    
    math::Vec2 GameScene::convertMousePosition(SDL_Event event)
    {
        auto posX = event.motion.x;
        auto posY = event.motion.y;
        auto deltaX = posX - kBoardStartPositionX;
        auto deltaY = (kBoardStartPositionY + kTileHeight) - posY;
        auto boardRightBoundary = kMaxNumberOfColumns * kTileWidth;
        auto boardTopBoundary = kMaxNumberOfRows  * kTileHeight;
        
        
        if (deltaX < 0 ||  deltaX > boardRightBoundary ||
            deltaY < 0 || deltaY >  boardTopBoundary){
            return Vec2(-1, -1);
        }
        
        auto selectColumn = deltaX / kTileWidth;
        auto selectRow = deltaY / kTileHeight;
        
        return math::Vec2(std::min(selectRow, kMaxNumberOfRows - 1),
                          std::min(selectColumn, kMaxNumberOfRows - 1));
    }
    
    math::Vec2 GameScene::convertTileCoordinatePosition(int row, int colum)
    {
        return math::Vec2(kBoardStartPositionX + colum * kTileWidth,
                          kBoardStartPositionY - row * kTileHeight);
    }
    
    void GameScene::animateFallingTiles(std::vector<std::vector<Tile*>> tiles)
    {
        for(auto && stack : tiles)
        {
            int index = 0;
            for(auto&& tile: stack)
            {
                auto newPosition = this->convertTileCoordinatePosition(tile->_row, tile->_column);
                auto delay = 0.1 + index * 0.15;
                
                auto duration = ((newPosition.y - tile->getSprite()->getPosition().y) / kTileHeight) * 0.1;

                auto action = _game->getActionManager()->addAction(tile->getSprite());
                action->setDestination(newPosition);
                action->setDuration(duration);
                action->setDelay(delay);
                action->start();
                
                ++index;
            }
        }
        
        
    }
    
    TileSprite* GameScene::getAvailableTileSprite()const
    {
        TileSprite* invisibleSprite = nullptr;
        for(auto&& sprite: _tileSprites)
        {
            if (!sprite->isVisible()) {
                invisibleSprite = sprite.get();
                break;
            }
        }
        
        assert(invisibleSprite != nullptr);
        return invisibleSprite;
    }
    
    void GameScene::swapTiles()
    {
        this->_gameBoard->detectPossibleMoves();
        
        if (_gameBoard->isPossibleMove(std::make_pair(_firstSelectedTile, _secondSelectedTile)))
        {
            _gameBoard->swap(_firstSelectedTile, _secondSelectedTile);
            _game->setState(Game::State::ValidSwap);
        }
        else
        {
            //invalid moves
            _gameBoard->invalidSwap(_firstSelectedTile, _secondSelectedTile);
            _game->setState(Game::State::InValidSwap);
        }
        _firstSelectedTile = nullptr;
        _secondSelectedTile = nullptr;
    }
    
    GameBoard* GameScene::getGameBoard()const
    {
        return _gameBoard.get();
    }
    
    void GameScene::handleEvent(SDL_Event event)
    {
        
        if (event.type == SDL_MOUSEMOTION && event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            if (_isValidClick) {
                //process drag
                auto selection = this->convertMousePosition(event);
                if (selection.x == -1 || selection.y == -1) {
                    return;
                }
                
                if (fabs(selection.x - _selectRow) == 1  ||
                    fabs(selection.y - _selectColumn) == 1) {
                    
                    if (fabs(selection.x - _selectRow) == 1  &&
                        fabs(selection.y - _selectColumn) == 1) {
                        return;
                    }
                    
                    
                    _firstSelectedTile = this->getTileSprite(_selectRow, _selectColumn);
                    _secondSelectedTile = this->getTileSprite(selection.x, selection.y);
                    
                    this->swapTiles();

                    _isValidClick = false;
                    
                }
                
            }

        }

        else if( event.type == SDL_MOUSEBUTTONDOWN )
        {
            auto selection = this->convertMousePosition(event);
            auto posX = event.motion.x;
            auto posY = event.motion.y;
            
            _selectColumn = selection.y;
            _selectRow = selection.x;
            if (_selectColumn == -1 || _selectRow == -1) {
                return;
            }
            _selectedTile = this->getTileSprite(_selectRow, _selectColumn);
            
            if (_selectedTile->getSprite()->hitTest(posX, posY)) {
                _isValidClick = true;
//                _gameBoard->print();
            }
            
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (_isValidClick) {
                //process click
                if (!_firstSelectedTile)
                {
                    _firstSelectedTile = _selectedTile;
                }
                else if (_selectedTile != _firstSelectedTile && !_secondSelectedTile)
                {
                    _secondSelectedTile = _selectedTile;
                    std::cout<<*_secondSelectedTile<<std::endl;
                    if (_firstSelectedTile->isAdjacent(_secondSelectedTile))
                    {
                        this->swapTiles();
                    }
                    else
                    {
                        _firstSelectedTile = _selectedTile;
                        _secondSelectedTile = nullptr;
                    }
                }
                
                _isValidClick = false;
            }

        }

        
    }
}
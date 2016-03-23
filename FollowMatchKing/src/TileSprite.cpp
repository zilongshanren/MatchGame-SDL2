//
//  TileSprite.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "TileSprite.hpp"

#include <assert.h>

#include "Constants.h"
#include "Game.hpp"
#include "ResourceManager.hpp"
#include "Tile.hpp"
#include "Action.hpp"
#include "ActionManager.hpp"
#include "GameScene.hpp"
#include "GameBoard.hpp"
#include "Game.hpp"
#include "Chain.hpp"

namespace king {
    TileSprite::TileSprite(Game* game , int type)
    :_game(game),
    _type(type)
    {
        this->setType(_type);
    }
    
    TileSprite::~TileSprite()
    {
        
    }
    
    void TileSprite::setType(int type)
    {
        //load textures.
        auto resourceManager = _game->getResourceManager();
        auto filename = g_rubyImageNames[type-1];
        auto texture = resourceManager->getImageForKey(filename);
        this->setTexture(texture);
    }
    
    void TileSprite::swap(king::TileSprite *other)
    {
        auto positionB = other->getPosition();
        auto actionManager = _game->getActionManager();
        auto actionA = actionManager->addAction(this);
        actionA->setDestination(positionB);
        actionA->start();
        
        auto positionA = this->getPosition();
        auto actionB = actionManager->addAction(other);
        actionB->setDestination(positionA);
        actionB->start();
    }
    
    void TileSprite::invalidSwap(king::TileSprite *other)
    {
        auto positionB = other->getPosition();
        auto actionManager = _game->getActionManager();
        auto actionA = actionManager->addAction(this);
        actionA->setDestination(positionB);
        actionA->start();
                
        auto positionA = this->getPosition();
        auto actionB = actionManager->addAction(other);
        actionB->setDestination(positionA);
        actionB->start();
        
        auto action_reverseA = actionManager->addAction(this);
        action_reverseA->setDestination(positionA);
        action_reverseA->setActive(false);
       
        //we could also encapsulate a sequence action
        actionA->setCallback([=](){
            action_reverseA->start();
        });
        
        auto action_reverseB = actionManager->addAction(other);
        action_reverseB->setDestination(positionB);
        action_reverseB->setActive(false);
        
        actionB->setCallback([=](){
            action_reverseB->start();
        });
        
    }
}
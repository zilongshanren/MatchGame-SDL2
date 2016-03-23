//
//  ActionManager.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/7/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "ActionManager.hpp"
#include "Action.hpp"
#include "Game.hpp"

namespace king {
    ActionManager::ActionManager(Game* game)
    :_game(game)
    {
        
    }
    
    ActionManager::~ActionManager()
    {
        for(auto&& action: _activeActions)
        {
            delete action;
        }
        _activeActions.clear();
        
        for(auto&& action: _inactiveActions)
        {
            delete action;
        }
        _inactiveActions.clear();
    }
    
    void ActionManager::update(float dt)
    {
        bool hasActions = false;
        std::list<Action*> markForRemove;
        
        for(auto&& action : _activeActions)
        {
            if (action->isActive()) {
                action->update(dt);
            }
            
            if (action->isDone()) {
                markForRemove.push_back(action);
            }
            hasActions = true;
        }
        
        for(auto&& action : markForRemove)
        {
            this->recyleAction(action);
        }
        
        if (!hasActions) {
            _game->stepToNextState();
        }
    }
    
    Action* ActionManager::addAction(king::GameObject *obj)
    {
        Action *action = nullptr;
        if (_inactiveActions.size() > 0) {
            action = _inactiveActions.front();
            _inactiveActions.remove(action);
            
            action->setTarget(obj);
        }
        if (action == nullptr) {
            action = new Action(obj);
        }
        action->setDuration(0.3f);
        action->setDuration(0.3f);
        action->setActive(false);
        action->setIsDone(false);
        action->setDelay(0);
        
        _activeActions.push_back(action);
        return action;
    }
    
    void ActionManager::recyleAction(king::Action *action)
    {
        _activeActions.remove(action);
        _inactiveActions.push_back(action);
    }
    
    
}
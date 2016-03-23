//
//  Action.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/6/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "Action.hpp"
#include <algorithm>
#include <limits>
#include <iostream>

#include "GameObject.hpp"
#include "TweenFunction.hpp"

namespace king {
    Action::Action(GameObject* object)
    :_target(object),
    _duration(0.3),
    _elapsedTime(0),
    _isDone(false),
    _isActive(true),
    _delay(0)
    {
        _tweenFunc = std::bind(math::easeOut, std::placeholders::_1, 2.5);
    }
    
    Action::~Action()
    {
        
    }
    
    bool Action::isDone() const
    {
        return _isDone;
    }
    
    void Action::setIsDone(bool done)
    {
        _isDone = done;
    }
    
    void Action::setActive(bool active)
    {
        _isActive = active;
    }
    
    bool Action::isActive()const
    {
        return _isActive;
    }
    
    void Action::setDelay(float seconds)
    {
        _delay = seconds;
    }
    
    void Action::update(float dt)
    {
        if (!_isDone)
        {
            _elapsedTime += dt;
            if (_delay > 0) {
                _delay = _delay - _elapsedTime;
                return;
            }
            
            auto totalDuration = std::max(_duration, std::numeric_limits<float>::epsilon());
            
            float delta = std::max(0.0f, std::min(1.0f, _elapsedTime / totalDuration));
            delta = _tweenFunc(delta);
            
            auto deltaPosition = _destination - _startPosition;
            auto offset = math::Vec2(deltaPosition.x * delta, deltaPosition.y * delta);
            _target->setPosition(_startPosition + offset);
            
            if (delta >= 1) {
                this->end();
                
                if (_callback) {
                    _callback();
                }
            }
        }
    }
    
    void Action::setCallback(std::function<void ()> func)
    {
        _callback = func;
    }
    
    void Action::start()
    {
        _startPosition = _target->getPosition();
        _isDone = false;
        _isActive = true;
    }
    
    void Action::end()
    {
        _isDone = true;
        _elapsedTime = 0;
        _isActive = false;
        _delay = 0;
    }
    
    void Action::setDestination(math::Vec2 destionation)
    {
        _destination = destionation;
    }
    
    void Action::setDuration(float seconds)
    {
        _duration = seconds;
    }
    
    void Action::setTarget(GameObject* object)
    {
        _target = object;
        _startPosition = _target->getPosition();
    }
    
    void Action::setTweenFunc(std::function<float (float)> func)
    {
        _tweenFunc = func;
    }
}
//
//  Action.hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/6/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef Action_hpp
#define Action_hpp

#include <functional>
#include "SimpleMath.h"

namespace king {
    class GameObject;
    
    class Action
    {
    public:
        Action(GameObject* obj);
        ~Action();
        
        void update(float dt);
        void setTarget(GameObject* object);
        void setDestination(math::Vec2 destionation);
        
        void setDuration(float seconds);
        void setDelay(float seconds);
        
        void setTweenFunc(std::function<float(float)> func);
        void setCallback(std::function<void()> func);
        void start();
        void end();
        bool isDone() const;
        void setIsDone(bool done);
        void setActive(bool active);
        bool isActive() const;
        
    private:
        std::function<float(float)> _tweenFunc;
        std::function<void()> _callback;
        GameObject* _target;
        math::Vec2 _destination;
        math::Vec2 _startPosition;
        float _duration;
        float _elapsedTime;
        float _delay;
        bool _isDone;
        bool _isActive;
    };
}

#endif /* Action_hpp */

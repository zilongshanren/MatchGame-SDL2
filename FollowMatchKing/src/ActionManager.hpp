//
//  ActionManager.hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/7/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef ActionManager_hpp
#define ActionManager_hpp
#include <list>

#include "base/Macros.h"

namespace king {
    class Action;
    class GameObject;
    class Game;
    
    class ActionManager
    {
    public:
        ActionManager(Game *game);
        ~ActionManager();
        
        Action* addAction(GameObject* obj);
        void recyleAction(Action* action);
        void update(float dt);
        
    private:
        KING_DISALLOW_COPY_AND_ASSIGN(ActionManager);
        std::list<Action*> _activeActions;
        std::list<Action*> _inactiveActions;
        Game* _game; //weak ref
    };
}

#endif /* ActionManager_hpp */

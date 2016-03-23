//
//  Game_hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <iostream>

#include <SDL2/SDL.h>

#include "base/Macros.h"

namespace king {
    class ResourceManager;
    class GameScene;
    class ActionManager;
    class GameBoard;

    class Game {
    public:
        enum class State
        {
            GameOver,
            Idle,
            ValidSwap,
            InValidSwap,
            DetectMatches,
            Falling,
            RemoveMatches,
            AddNewTiles
        };
        Game();
        ~Game();
        
        void init();
        
        SDL_Renderer* getRenderer() const;
        SDL_Window* getWindow() const;
        bool isRunning() const;
        void setIsRunning(bool running);
        float getCurrentTime() const;
        ResourceManager* getResourceManager() const;
        ActionManager* getActionManager() const;
        GameScene* getGameScene() const;
        
        void setFPS(int fps);
        int getFPS() const;
        void setUPS(int ups);
        int getUPS() const;
        
        void handleEvent(SDL_Event e);
        void update(float dt);
        void render();
        
        void run();

        void stepToNextState();
        void setState(State state);
        State getCurrentState() const;
        
    protected:
        void preloadImages();
        
    private:
        KING_DISALLOW_COPY_AND_ASSIGN(Game);
        SDL_Renderer* _renderer;
        SDL_Window* _window;
        bool _isRunning;
        int _FPS;
        int _UPS;
        
        std::unique_ptr<ResourceManager> _resourceManager;
        std::unique_ptr<GameScene> _gameScene;
        std::unique_ptr<ActionManager> _actionManager;
        State _state;
    };
}

#endif /* Game_hpp */

//
//  Game.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "Game.hpp"

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "utils/Utils.h"
#include "Constants.h"
#include "ResourceManager.hpp"
#include "GameScene.hpp"
#include "ActionManager.hpp"


namespace king {
    Game::Game()
    :_renderer(nullptr),
    _window(nullptr),
    _isRunning(true),
    _FPS(60),
    _UPS(0),
    _state(State::Idle)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0){
            king::utils::logSDLError(std::cout, "SDL_Init");
        }
        
        int imageInitFlag = IMG_INIT_PNG | IMG_INIT_JPG;
        if ((IMG_Init(imageInitFlag) & imageInitFlag) != imageInitFlag){
            king::utils::logSDLError(std::cout, "IMG_Init");
        }
        
        if (TTF_Init() != 0){
            king::utils::logSDLError(std::cout, "TTF_Init");
        }
        
        this->init();
    }
    
    void Game::preloadImages()
    {
        _resourceManager->addImage(kBackgroundImageName);
        int totalCount = sizeof(g_rubyImageNames) / sizeof(g_rubyImageNames[0]);
        for(int i = 0; i < totalCount; ++i)
        {
            _resourceManager->addImage(g_rubyImageNames[i]);
        }
    }
    
    Game::~Game()
    {
        utils::EasyCleanup(_renderer, _window);
    }
    
    void Game::init()
    {
        _window = SDL_CreateWindow("FollowMatchKing", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (_window == nullptr){
            king::utils::logSDLError(std::cout, "CreateWindow");
        }
        
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (_renderer == nullptr){
            king::utils::logSDLError(std::cout, "CreateRenderer");
        }
        
        _resourceManager = std::unique_ptr<ResourceManager>(new ResourceManager(_renderer));

        this->preloadImages();
        
        _gameScene = std::unique_ptr<GameScene>(new GameScene(this));
        
        _actionManager = std::unique_ptr<ActionManager>(new ActionManager(this));
    }
    
    ResourceManager* Game::getResourceManager() const
    {
        return _resourceManager.get();
    }
    
    GameScene* Game::getGameScene()const
    {
        return _gameScene.get();
    }
    
    ActionManager* Game::getActionManager() const
    {
        return _actionManager.get();
    }
    
    SDL_Window* Game::getWindow() const
    {
        return _window;
    }
    
    SDL_Renderer* Game::getRenderer() const
    {
        return _renderer;
    }
    
    bool Game::isRunning() const
    {
        return _isRunning;
    }
    
    void Game::setIsRunning(bool running)
    {
        _isRunning = running;
    }
    
    float Game::getCurrentTime() const
    {
        return SDL_GetTicks() / 1000.0f;
    }
    
    void Game::setFPS(int fps)
    {
        _FPS = fps;
    }
    
    int Game::getFPS() const
    {
        return _FPS;
    }
    
    void Game::setUPS(int ups)
    {
        _UPS = ups;
    }
    
    int Game::getUPS() const
    {
        return _UPS;
    }
    
    void Game::update(float dt)
    {
        if (_state != State::Idle) {
            _actionManager->update(dt);
        }
        _gameScene->update(dt);
    }
    
    
    void Game::render()
    {
        SDL_RenderClear(_renderer);
        
        _gameScene->render(_renderer);
        
        SDL_RenderPresent(_renderer);
    }
    
    void Game::handleEvent(SDL_Event e)
    {
        if (e.type == SDL_QUIT){
            _isRunning = false;
            return;
        }
        
        if (_state == State::Idle && _state != State::GameOver) {
            _gameScene->handleEvent(e);
        }
    }
    
    void Game::setState(king::Game::State state)
    {
        _state = state;
    }
    
    void Game::stepToNextState()
    {
        switch (_state) {
            case State::InValidSwap:
                _state = State::Idle;
                break;
            case State::ValidSwap:
                _state = State::RemoveMatches;
                break;
            case State::RemoveMatches:
                _state = State::Falling;
                break;
            case State::Falling:
                _state = State::AddNewTiles;
                break;
            case State::AddNewTiles:
                _state = State::RemoveMatches;
                break;
            default:
                break;
        }
        _gameScene->handleGameState();
    }
    
    Game::State Game::getCurrentState() const
    {
        return _state;
    }
    
    void Game::run()
    {
        double currentTime = this->getCurrentTime();
        double startTime = this->getCurrentTime();
        const double dt = 1.0 / 30;  //update 30 times per second
        double accumulator = 0.0;
        double elapseMS = 0.0;
        
        int numUpdates = 0;
        int numFrames = 0;
        
        //Our event structure
        SDL_Event e;
        while (this->isRunning()){
            
            while (SDL_PollEvent(&e)){
                this->handleEvent(e);
            }
            
            float newTime = this->getCurrentTime();
            float frameTime = (newTime - currentTime);
            if (frameTime > 0.25f) {
                frameTime = 0.25;
            }
            
            currentTime = newTime;
            
            accumulator += frameTime;
            
            while (accumulator >= dt) {
                this->update(dt);
                accumulator -= dt;
                numUpdates++;
            }
            this->render();
            numFrames++;
            
            elapseMS = this->getCurrentTime() - startTime;
            if (elapseMS >= 1) {
                this->setFPS(numFrames);
                this->setUPS(numUpdates);
                startTime = this->getCurrentTime();
                numUpdates = 0;
                numFrames = 0;
            }
        }

    }
    
}
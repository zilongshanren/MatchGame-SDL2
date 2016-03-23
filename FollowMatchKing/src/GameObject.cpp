//
//  GameObject.cpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "GameObject.hpp"


namespace king {
    GameObject::GameObject()
    :_texture(nullptr),
    _boundingBox({0,0,0,0}),
    _visible(true)
    {
        
    }
    
    GameObject::~GameObject()
    {
        
    }
    
    void GameObject::setPosition(math::Vec2 position)
    {
        _boundingBox.x = position.x;
        _boundingBox.y = position.y;
    }
    
    math::Vec2 GameObject::getPosition() const
    {
        return math::Vec2(_boundingBox.x, _boundingBox.y);
    }
    
    void GameObject::setSize(math::Size size)
    {
        _boundingBox.w = size.width;
        _boundingBox.h = size.height;
    }
    
    math::Size GameObject::getSize() const
    {
        return math::Size(_boundingBox.w, _boundingBox.h);
    }
    
    bool GameObject::hitTest(int x, int y)
    {
        if (x >= _boundingBox.x && (x <= _boundingBox.x + _boundingBox.w)
            && y >= _boundingBox.y && (y <= _boundingBox.y + _boundingBox.h)) {
            return true;
        }
        return false;
    }
    
    void GameObject::setTexture(SDL_Texture *texture)
    {
        _texture = texture;
        int w, h;
        SDL_QueryTexture(_texture, nullptr, nullptr, &w, &h);
        _boundingBox.w = w;
        _boundingBox.h = h;
    }
    
    void GameObject::setVisible(bool visible)
    {
        _visible = visible;
    }
    
    bool GameObject::isVisible() const
    {
        return _visible;
    }
    
    void GameObject::render(SDL_Renderer *renderer)
    {
        if (_visible && _texture)
        {
            SDL_RenderCopy(renderer, _texture, nullptr, &_boundingBox);
        }
    }
    
}
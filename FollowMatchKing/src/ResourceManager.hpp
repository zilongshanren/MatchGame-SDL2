//
//  ResourceManager.hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include <vector>
#include <string>
#include <unordered_map>

#include "base/Macros.h"

//http://stackoverflow.com/questions/7222143/unordered-map-hash-function-c

template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
    template<typename S, typename T> struct hash<pair<S, T>>
    {
        inline size_t operator()(const pair<S, T> & v) const
        {
            size_t seed = 0;
            ::hash_combine(seed, v.first);
            ::hash_combine(seed, v.second);
            return seed;
        }
    };
}

class SDL_Renderer;
class SDL_Texture;
// TTF_Font is a alias to _TTF_Font
class _TTF_Font;

namespace king {
    class ResourceManager {
    public:
        explicit ResourceManager(SDL_Renderer* renderer);
        ~ResourceManager();
        
        SDL_Texture* addImage(const std::string& filename);
        SDL_Texture* getImageForKey(const std::string& filename);
        
        _TTF_Font* addTTFFont(const std::string& filename, int fontSize);
        _TTF_Font* getTTFFontForKey(const std::string& filename, int fontSize);
    private:
        KING_DISALLOW_COPY_AND_ASSIGN(ResourceManager);
        
        std::unordered_map<std::string, SDL_Texture*> _textures;
        std::unordered_map<std::pair<std::string, int>, _TTF_Font*> _ttfFonts;
        SDL_Renderer* _renderer;
    };
}


#endif /* ResourceManager_hpp */

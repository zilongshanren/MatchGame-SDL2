//
//  SimpleMath.h
//  FollowMatchKing
//
//  Created by guanghui on 3/5/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef SimpleMath_h
#define SimpleMath_h

namespace king {
    namespace math {
        class Vec2
        {
        public:
            float x;
            float y;
            
            Vec2()
            :x(0),y(0)
            {}
            
            Vec2(float x, float y)
            :x(x), y(y)
            {}
            
            inline const Vec2 operator+(const Vec2& v) const
            {
                return Vec2(this->x + v.x, this->y + v.y);
            }
  
            inline Vec2& operator+=(const Vec2& v)
            {
                this->x += v.x;
                this->y += v.y;
                return *this;
            }

            inline const Vec2 operator-(const Vec2& v) const
            {
                return Vec2(this->x - v.x, this->y - v.y);
            }
   
            inline Vec2& operator-=(const Vec2& v)
            {
                this->x -= v.x;
                this->y -= v.y;
                return *this;
            }
        };
        
        class Size
        {
        public:
            float width;
            float height;
            
            Size()
            :width(0),height(0)
            {}
            
            Size(float width, float height)
            :width(width),height(height)
            {}
        };
        
        class Rect
        {
        public:
            Vec2 origin;
            Size size;
            
            Rect(){}
            
            Rect(float x, float y, float width, float height)
            :origin(x, y),size(width, height)
            {}
        };
    }
}

#endif /* SimpleMath_h */

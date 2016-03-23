//
//  TweenFunction.hpp
//  FollowMatchKing
//
//  Created by guanghui on 3/6/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#ifndef TweenFunction_hpp
#define TweenFunction_hpp

#include <cmath>

namespace king {
    namespace math {
        
        inline float easeOut(float time, float rate)
        {
            return powf(time, 1 / rate);
        }

        // Linear
        inline float linear(float time)
        {
            return time;
        }

    }
}

#endif /* TweenFunction_hpp */

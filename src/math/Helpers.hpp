#pragma once

#include <cmath>
#include <numbers>

namespace math
{

    inline float wrapAngle(float angle)
    {
        constexpr float twoPi = 2.0f * std::numbers::pi_v<float>;

        angle = std::fmod(angle, twoPi);

        if (angle < 0.0f)
            angle += twoPi;

        return angle;
    }

}
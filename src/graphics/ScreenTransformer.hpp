
#pragma once

#include "math/Vector.h"


namespace graphics
{
class ScreenTransformer {
    public:
    ScreenTransformer() = delete;

    static constexpr math::Vec3f& transformVector(math::Vec3f& initialVector, unsigned width, unsigned height) {
        const float x = initialVector.elements[0];
        const float y = initialVector.elements[1];
        const float zInv = 1.0f / initialVector.elements[2];

        initialVector.elements[0] =  (x * zInv + 1.0f) * static_cast<float>(width) / 2.0f;
        initialVector.elements[1] = (1.0f - y * zInv) * static_cast<float>(height) / 2.0f;

        return initialVector;
    }

   [[nodiscard]] static constexpr math::Vec3f getTransformed(const math::Vec3f& initialVector, unsigned width, unsigned height) {
       math::Vec3f transformedVector(initialVector);
       return transformVector(transformedVector, width, height);
    }

};
}
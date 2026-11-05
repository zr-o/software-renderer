#include "math/Vector.hpp"

namespace math
{
[[nodiscard]] inline float edgeFunction(const math::Vec2f &start,
                                        const math::Vec2f &end,
                                        const math::Vec2f &point)
{
    return ((end.x() - start.x()) * (point.y() - start.y()) -
            (end.y() - start.y()) * (point.x() - start.x()));
}
} // namespace math

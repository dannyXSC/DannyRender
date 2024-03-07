#ifndef __DANNY__GEOMETRY__BBOX__
#define __DANNY__GEOMETRY__BBOX__
#include <core/forward_decl.h>
#include <geometry/Ray.hpp>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <cmath>
#include <array>

namespace danny
{
    namespace geometry
    {
        class BBox
        {
        public:
            BBox() = default;
            BBox(const glm::vec3 &min, const glm::vec3 &max);

            void extend(const glm::vec3 &point);
            void extend(const BBox &bbox);
            float getSurfaceArea() const;
            glm::vec2 intersect(const glm::vec3 &origin, const glm::vec3 &inv_dir) const;

            const glm::vec3 &get_min() const { return m_min; }
            const glm::vec3 &get_max() const { return m_max; }

        private:
            glm::vec3 m_min{std::numeric_limits<float>::max()};
            glm::vec3 m_max{-std::numeric_limits<float>::max()};
        };
    }
}
#endif
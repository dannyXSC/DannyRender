#ifndef __DANNY__GEOMETRY__RAY__
#define __DANNY__GEOMETRY__RAY__
#include <core/forward_decl.h>

#include <glm/vec3.hpp>

#include <ostream>
namespace danny
{
    namespace geometry
    {
        class Ray
        {
        public:
            glm::vec3 origin;
            glm::vec3 direction, direction_inv;

            Ray(const glm::vec3 &ori, const glm::vec3 &dir) : origin(ori), direction(dir)
            {
                direction_inv = glm::vec3(1. / direction.x, 1. / direction.y, 1. / direction.z);
            }

            glm::vec3 operator()(double t) const { return origin + direction * glm::vec3(t); }
        };
    }
}
#endif
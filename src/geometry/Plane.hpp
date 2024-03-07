#ifndef __DANNY__GEOMETRY__PLANE__
#define __DANNY__GEOMETRY__PLANE__
#include <core/forward_decl.h>

#include <glm/vec3.hpp>

#include <geometry/Ray.hpp>
namespace danny
{
    namespace geometry
    {
        class Plane
        {
        public:
            glm::vec3 point;
            glm::vec3 normal;

            Plane() = default;
            // p_normal is assumed to be normalized.
            Plane(const glm::vec3 &p_point, const glm::vec3 &p_normal) : point(p_point), normal(p_normal){};

            // Returns the point intersected on the plane.
            glm::vec3 intersect(const Ray &ray);
        };
    }
}
#endif
#include "Plane.hpp"

#include <glm/geometric.hpp>
namespace danny
{
    namespace geometry
    {
        glm::vec3 Plane::intersect(const Ray &ray)
        {
            auto distance = glm::dot((ray.origin - this->point), ray.direction);
            return ray(distance);
        }
    }
}
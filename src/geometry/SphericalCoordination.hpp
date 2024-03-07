#ifndef __DANNY__GEOMETRY__SPHERICALCOORDINATE__
#define __DANNY__GEOMETRY__SPHERICALCOORDINATE__

#include <glm/vec3.hpp>

namespace danny
{
    namespace geometry
    {
        struct SphericalCoordinate
        {
            float radius;
            float theta;
            float phi;

            explicit SphericalCoordinate(const glm::vec3 &cartesian_coordinate);
            SphericalCoordinate(float p_radius, float p_theta, float p_phi);

            glm::vec3 toCartesianCoordinate() const;
        };
    }
}

#endif
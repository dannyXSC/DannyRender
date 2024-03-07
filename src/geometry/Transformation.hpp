#ifndef __DANNY__GEOMETRY__TRANSFORMATION__
#define __DANNY__GEOMETRY__TRANSFORMATION__
#define GLM_ENABLE_EXPERIMENTAL

#include <core/forward_decl.h>

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

#include <geometry/Ray.hpp>
#include <geometry/Plane.hpp>

namespace danny
{
    namespace geometry
    {
        class Transformation
        {
        public:
            struct Info
            {
                glm::vec3 scaling{1., 1., 1.};
                glm::vec3 rotation{0., 0., 0.};
                glm::vec3 translation{0., 0., 0.};
            };

        public:
            explicit Transformation(Transformation::Info &info);

            glm::vec3 vectorToObjectSpace(const glm::vec3 &vector) const;
            glm::vec3 vectorToWorldSpace(const glm::vec3 &vector) const;
            glm::vec3 pointToObjectSpace(const glm::vec3 &point) const;
            glm::vec3 pointToWorldSpace(const glm::vec3 &point) const;
            glm::vec3 normalToWorldSpace(const glm::vec3 &normal) const;
            geometry::Ray rayToObjectSpace(const geometry::Ray &ray) const;
            geometry::Plane planeToWorldSpace(const geometry::Plane &plane) const;

        private:
            glm::mat3 m_transformation;
            glm::mat3 m_inverse_transformation;
            glm::vec3 m_translation;
        };
    }
}

#endif
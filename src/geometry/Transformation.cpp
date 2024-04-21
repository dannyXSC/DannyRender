#include "Transformation.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace danny
{
    namespace geometry
    {
        Transformation::Xml::Xml()
            : info(glm::vec3(1.0), glm::vec3(0.0), glm::vec3(0.0))
        {
        }

        Transformation::Xml::Xml(const xml::Node &node)
        {
            node.parseChildText("Scaling", &info.scaling.x, 1.0f, &info.scaling.y, 1.0f, &info.scaling.z, 1.0f);
            node.parseChildText("Rotation", &info.rotation.x, 0.0f, &info.rotation.y, 0.0f, &info.rotation.z, 0.0f);
            node.parseChildText("Translation", &info.translation.x, 0.0f, &info.translation.y, 0.0f, &info.translation.z, 0.0f);

            if (node.parent().attribute("type") == std::string("Sphere"))
            {
                if (!(info.scaling.x == info.scaling.y && info.scaling.y == info.scaling.z))
                {
                    node.child("Scaling").throwError("Non-uniform scaling is not allowed for spheres.");
                }
            }
        }

        Transformation::Transformation(const Transformation::Info &info)
            : m_transformation(glm::orientate4(glm::radians(glm::vec3(info.rotation.x, info.rotation.z, info.rotation.y))) * glm::scale(info.scaling)),
              m_inverse_transformation(glm::inverse(m_transformation)),
              m_translation(info.translation)
        {
        }

        Transformation::Transformation(const Transformation::QuatInfo &info)
            : m_transformation(glm::mat4_cast(glm::angleAxis(glm::radians(info.rotation.w), glm::normalize(glm::vec3(info.rotation)))) * glm::scale(info.scaling)),
              m_inverse_transformation(glm::inverse(m_transformation)),
              m_translation(info.translation)
        {
        }

        glm::vec3 Transformation::vectorToObjectSpace(const glm::vec3 &vector) const
        {
            return m_inverse_transformation * vector;
        }

        glm::vec3 Transformation::vectorToWorldSpace(const glm::vec3 &vector) const
        {
            return m_transformation * vector;
        }

        glm::vec3 Transformation::pointToObjectSpace(const glm::vec3 &point) const
        {
            return m_inverse_transformation * (point - m_translation);
        }

        glm::vec3 Transformation::pointToWorldSpace(const glm::vec3 &point) const
        {
            return m_transformation * point + m_translation;
        }

        glm::vec3 Transformation::normalToWorldSpace(const glm::vec3 &normal) const
        {
            return glm::transpose(m_inverse_transformation) * normal;
        }

        geometry::Ray Transformation::rayToObjectSpace(const geometry::Ray &ray) const
        {
            return geometry::Ray(pointToObjectSpace(ray.origin), vectorToObjectSpace(ray.direction));
        }

        geometry::Plane Transformation::planeToWorldSpace(const geometry::Plane &plane) const
        {
            return geometry::Plane(pointToWorldSpace(plane.point), glm::normalize(normalToWorldSpace(plane.normal)));
        }
    }
}
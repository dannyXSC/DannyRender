#ifndef __DANNY__GEOMETRY__TRANSFORMATION__
#define __DANNY__GEOMETRY__TRANSFORMATION__
#define GLM_ENABLE_EXPERIMENTAL

#include <core/forward_decl.h>

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

#include <xml/Node.h>
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

                Info() {}

                Info(const glm::vec3 &scaling, const glm::vec3 &rotation, const glm::vec3 &translation)
                    : scaling(scaling), rotation(rotation), translation(translation) {}

                Info(const Info &info)
                {
                    scaling = info.scaling;
                    rotation = info.rotation;
                    translation = info.translation;
                }
            };

            struct QuatInfo
            {
                glm::vec3 scaling{1., 1., 1.};
                // quaternion to represent rotation {x , y , z , theta}
                glm::vec4 rotation{0., 0., 0., 0.};
                glm::vec3 translation{0., 0., 0.};
            };

        public:
            // Xml structure of the class.
            struct Xml
            {
                // glm::vec3 scaling;
                // glm::vec3 rotation;
                // glm::vec3 translation;

                Info info;

                Xml();
                explicit Xml(const xml::Node &node);
            };

        public:
            explicit Transformation(const Transformation::Xml &xml) : Transformation(xml.info){};
            explicit Transformation(const Transformation::Info &info);
            explicit Transformation(const Transformation::QuatInfo &info);

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
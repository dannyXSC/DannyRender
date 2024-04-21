#ifndef __DANNY__CORE__PINHOLECAMERA__
#define __DANNY__CORE__PINHOLECAMERA__

#include <xml/Node.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <core/Camera.h>
#include <core/CoordinateSpace.h>

namespace danny
{
    namespace core
    {
        class PinholeCamera : public Camera
        {
        public:
            // Xml structure of the class.
            struct Xml : Camera::Xml
            {
                glm::vec3 position;
                glm::vec3 direction;
                glm::vec3 up;
                glm::vec2 fov_xy;
                glm::ivec2 resolution;
                float near_distance;

                explicit Xml(const xml::Node &node);
                std::unique_ptr<Camera> create() const override;
            };

        public:
            explicit PinholeCamera(const PinholeCamera::Xml &xml)
                : PinholeCamera(xml.resolution,
                                xml.near_distance,
                                xml.fov_xy,
                                xml.position,
                                xml.direction,
                                xml.up){};
            explicit PinholeCamera(const glm::ivec2 &resolution,
                                   float near_distance,
                                   const glm::vec2 &fov_xy,
                                   const glm::vec3 &pos = glm::vec3(0., 0., 0.),
                                   const glm::vec3 &dir = glm::vec3(0., 0., -1.),
                                   const glm::vec3 &up = glm::vec3(0., 1., 0.));

            geometry::Ray castRay(int x, int y, float offset_x = 0.5f, float offset_y = 0.5f) const override;
            const glm::ivec2 &get_resolution() const override { return m_resolution; };

        private:
            CoordinateSpace m_camera_space;
            glm::vec4 m_screen_coordinates;
            glm::ivec2 m_resolution;
            glm::vec2 m_pixel_length;
            float m_near_distance;
        };
    }
}

#endif

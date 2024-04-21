#ifndef __DANNY__CORE__CAMERA__
#define __DANNY__CORE__CAMERA__

#include <memory>

#include <glm/vec2.hpp>

#include <xml/Node.h>
#include <geometry/Ray.hpp>

namespace danny
{
    namespace core
    {
        class Camera
        {
        public:
            // Xml structure of the class.
            struct Xml
            {
                virtual ~Xml() = default;
                virtual std::unique_ptr<Camera> create() const = 0;
                static std::unique_ptr<Camera::Xml> factory(const xml::Node &node);
            };

        public:
            virtual ~Camera() = default;

            virtual geometry::Ray castRay(int x, int y, float offset_x = 0.5f, float offset_y = 0.5f) const = 0;
            virtual const glm::ivec2 &get_resolution() const = 0;
        };
    }
}

#endif

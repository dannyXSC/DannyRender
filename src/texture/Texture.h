#ifndef __DANNY__TEXTURE__TEXTURE__
#define __DANNY__TEXTURE__TEXTURE__

#include <glm/vec3.hpp>

#include <xml/Node.h>
#include <geometry/Intersection.hpp>

namespace danny
{
    namespace texture
    {
        class Texture
        {
        public:
            // Xml structure of the class.
            struct Xml
            {
                virtual ~Xml() = default;
                virtual std::unique_ptr<Texture> create() const = 0;
                static std::unique_ptr<Texture::Xml> factory(const xml::Node &node);
            };

        public:
            virtual ~Texture() = default;

            virtual glm::vec3 fetch(const geometry::Intersection &intersection) const = 0;
        };
    }
}

#endif
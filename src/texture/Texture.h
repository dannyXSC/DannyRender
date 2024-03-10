#ifndef __DANNY__TEXTURE__TEXTURE__
#define __DANNY__TEXTURE__TEXTURE__

#include <glm/vec3.hpp>

#include <geometry/Intersection.hpp>

namespace danny
{
    namespace texture
    {
        class Texture
        {
        public:
            virtual ~Texture() = default;

            virtual glm::vec3 fetch(const geometry::Intersection &intersection) const = 0;
        };
    }
}

#endif
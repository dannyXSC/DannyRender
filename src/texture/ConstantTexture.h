#ifndef __DANNY__TEXTURE__CONSTANTTEXTURE__
#define __DANNY__TEXTURE__CONSTANTTEXTURE__

#include <glm/vec3.hpp>

#include <texture/Texture.h>

namespace danny
{
    namespace texture
    {
        class ConstantTexture : public Texture
        {
        public:
            explicit ConstantTexture(const glm::vec3 &value) : m_value(value){};

            glm::vec3 fetch(const geometry::Intersection &intersection) const override;

        private:
            glm::vec3 m_value;
        };
    }
}

#endif
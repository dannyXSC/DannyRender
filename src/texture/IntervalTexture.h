#ifndef __DANNY__TEXTURE__INTERVALTEXTURE__
#define __DANNY__TEXTURE__INTERVALTEXTURE__

#include <glm/vec3.hpp>

#include <texture/Texture.h>

namespace danny
{
    namespace texture
    {
        class IntervalTexture : public Texture
        {
        public:
            explicit IntervalTexture(){};

            glm::vec3 fetch(const geometry::Intersection &intersection) const override;

        private:
            // glm::vec3 m_value;
        };
    }
}

#endif
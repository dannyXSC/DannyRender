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
            // Xml structure of the class.
            struct Xml : public Texture::Xml
            {
                explicit Xml(const xml::Node &node);
                std::unique_ptr<Texture> create() const override;
            };

        public:
            explicit IntervalTexture(const IntervalTexture::Xml &xml){};
            explicit IntervalTexture(){};

            glm::vec3 fetch(const geometry::Intersection &intersection) const override;

        private:
            // glm::vec3 m_value;
        };
    }
}

#endif
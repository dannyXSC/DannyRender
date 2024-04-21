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
            // Xml structure of the class.
            struct Xml : public Texture::Xml
            {
                glm::vec3 value;

                explicit Xml(const xml::Node &node);
                explicit Xml(const glm::vec3 &p_value);
                std::unique_ptr<Texture> create() const override;
            };

        public:
            explicit ConstantTexture(const ConstantTexture::Xml &xml) : ConstantTexture(xml.value){};
            explicit ConstantTexture(const glm::vec3 &value) : m_value(value){};

            glm::vec3 fetch(const geometry::Intersection &intersection) const override;

        private:
            glm::vec3 m_value;
        };
    }
}

#endif
#ifndef __DANNY__TEXTURE__IMAGETEXTURE__
#define __DANNY__TEXTURE__IMAGETEXTURE__

#include <string>

#include <glm/vec3.hpp>

#include <texture/Texture.h>

namespace danny
{
    namespace texture
    {
        class ImageTexture : public Texture
        {
        public:
            // Xml structure of the class.
            struct Xml : public Texture::Xml
            {
                std::string datapath;

                explicit Xml(const xml::Node &node);
                std::unique_ptr<Texture> create() const override;
            };

        public:
            explicit ImageTexture(const ImageTexture::Xml &xml) : ImageTexture(xml.datapath){};
            explicit ImageTexture(const std::string &path);

            glm::vec3 fetch(const geometry::Intersection &intersection) const override;
            glm::vec3 fetchTexelNearest(const glm::vec2 &uv, int mipmap_level) const;
            int getWidth() const;
            int getHeight() const;

        private:
            std::shared_ptr<std::vector<core::Image>> m_images;
        };
    }
}

#endif
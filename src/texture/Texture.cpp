#include "Texture.h"

#include "ConstantTexture.h"
#include "IntervalTexture.h"
#include "ImageTexture.h"

namespace danny
{
    namespace texture
    {
        std::unique_ptr<Texture::Xml> Texture::Xml::factory(const xml::Node &node)
        {
            auto texture_type = node.attribute("textureType", true);

            if (texture_type == std::string("Constant"))
            {
                return std::make_unique<ConstantTexture::Xml>(node);
            }
            else if (texture_type == std::string("Interval"))
            {
                return std::make_unique<IntervalTexture::Xml>(node);
            }
            else if (texture_type == std::string("Image"))
            {
                return std::make_unique<ImageTexture::Xml>(node);
            }
            else
            {
                node.throwError("Unknown Texture type.");
            }
            return nullptr;
        }
    }
}
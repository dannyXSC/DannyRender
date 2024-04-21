#include "IntervalTexture.h"

namespace danny
{
    namespace texture
    {
        IntervalTexture::Xml::Xml(const xml::Node &node)
        {
        }

        std::unique_ptr<Texture> IntervalTexture::Xml::create() const
        {
            return std::make_unique<IntervalTexture>(*this);
        }

        glm::vec3 IntervalTexture::fetch(const geometry::Intersection &intersection) const
        {
            auto uv = intersection.uv;
            int u = uv[0] * 16;
            int v = uv[1] * 16;
            if (((u + v) % 2) == 0)
                return glm::vec3(0.22, 0.72, 0.52);
            else
                return glm::vec3(1, 0.334, 0.52);
        }
    }
}
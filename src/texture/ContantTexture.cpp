#include "ConstantTexture.h"

namespace danny
{
    namespace texture
    {
        glm::vec3 ConstantTexture::fetch(const geometry::Intersection &intersection) const
        {
            return m_value;
        }
    }
}
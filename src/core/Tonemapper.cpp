#include "Tonemapper.h"

#include <glm/glm.hpp>

namespace danny
{
    namespace core
    {
        Clamp::Clamp(float min, float max)
            : m_min(min), m_max(max)
        {
        }

        Image Clamp::tonemap(const Image &image) const
        {
            auto tonemapped_image = image;
            int width = tonemapped_image.get_width();
            int height = tonemapped_image.get_height();

            for (int i = 0; i < width; ++i)
            {
                for (int j = 0; j < height; ++j)
                {
                    auto pixel = tonemapped_image.get(i, j);
                    tonemapped_image.set(i, j, glm::clamp(pixel, m_min, m_max));
                }
            }

            return tonemapped_image;
        }
    }
}
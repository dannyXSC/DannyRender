#include "Tonemapper.h"

#include <glm/glm.hpp>
#include <core/Math.hpp>

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

        GlobalReinhard::GlobalReinhard(float key, float max_luminance)
            : m_key(key), m_max_luminance(max_luminance)
        {
        }

        Image GlobalReinhard::tonemap(const Image &image) const
        {
            auto tonemapped_image = image;
            int width = tonemapped_image.get_width();
            int height = tonemapped_image.get_height();

            constexpr auto epsilon = 0.0001f;
            auto geometric_average = 0.0f;
            auto max_l_t = 0.0f;
            for (int i = 0; i < width; ++i)
            {
                for (int j = 0; j < height; ++j)
                {
                    auto pixel = tonemapped_image.get(i, j);

                    auto luminance = math::rgbToLuminance(pixel);
                    max_l_t = glm::max(luminance, max_l_t);

                    geometric_average += glm::log(epsilon + luminance);
                }
            }
            geometric_average = glm::exp(geometric_average / (width * height));
            auto scale = m_key / geometric_average;

            max_l_t *= (scale * m_max_luminance);

            for (int i = 0; i < width; ++i)
            {
                for (int j = 0; j < height; ++j)
                {
                    auto pixel = tonemapped_image.get(i, j);

                    auto l_t = scale * math::rgbToLuminance(pixel);

                    pixel *= scale * (1.0f + l_t / (max_l_t * max_l_t)) / (1.0f + l_t);
                    tonemapped_image.set(i, j, glm::clamp(pixel, 0.0f, 1.0f));
                }
            }

            return tonemapped_image;
        }
    }
}
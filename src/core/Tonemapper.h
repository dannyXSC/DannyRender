#ifndef __DANNY__CORE__TONEMAPPER__
#define __DANNY__CORE__TONEMAPPER__

#include <core/Image.h>

namespace danny
{
    namespace core
    {
        class Tonemapper
        {
        public:
            virtual ~Tonemapper() = default;

            virtual Image tonemap(const Image &image) const = 0;
        };

        // Clamps the pixel values that are out of the range to min and max.
        class Clamp : public Tonemapper
        {
        public:
            explicit Clamp(float min, float max);

            Image tonemap(const Image &image) const override;

        private:
            float m_min;
            float m_max;
        };
    }
}
#endif
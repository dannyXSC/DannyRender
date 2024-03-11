#ifndef __DANNY__MICROFACET__FRESNEL__
#define __DANNY__MICROFACET__FRESNEL__

#include <glm/vec3.hpp>

namespace danny
{
    namespace microfacet
    {
        namespace fresnel
        {
            struct Dielectric
            {
                float operator()(float nt_over_ni, float costheta);
            };

            struct Conductor
            {
                glm::vec3 operator()(const glm::vec3 &nt_over_ni, const glm::vec3 &kt_over_ki, float costheta);
            };
        }
    }
}

#endif

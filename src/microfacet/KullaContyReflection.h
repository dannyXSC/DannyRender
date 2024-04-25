#ifndef __DANNY__MICROFACET__KULLACONTY__
#define __DANNY__MICROFACET__KULLACONTY__

#include <memory>

#include <glm/vec3.hpp>

#include <core/forward_decl.h>

namespace danny
{
    namespace microfacet
    {
        class KullaConty
        {
        public:
            explicit KullaConty(std::unique_ptr<microfacet::MicrofacetReflection> microfacet,
                                std::unique_ptr<texture::Texture> precomputed_e,
                                std::unique_ptr<texture::Texture> precomputed_e_avg,
                                const glm::vec3 &r, const glm::vec3 &g);

            glm::vec3 getBsdf(float roughness, const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent) const;

        private:
            std::unique_ptr<microfacet::MicrofacetReflection> m_microfacet;
            std::unique_ptr<texture::Texture> m_precomputed_e;
            std::unique_ptr<texture::Texture> m_precomputed_e_avg;
            glm::vec3 m_fresnel_avg;
        };
    }
}
#endif
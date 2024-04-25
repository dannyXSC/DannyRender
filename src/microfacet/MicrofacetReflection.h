#ifndef __DANNY__MICROFACET__MICROFACETREFLECTION__
#define __DANNY__MICROFACET__MICROFACETREFLECTION__

#include <utility>
#include <memory>

#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include <core/forward_decl.h>
#include <core/Sampler.hpp>
#include <microfacet/Microfacet.h>

namespace danny
{
    namespace microfacet
    {
        class MicrofacetReflection : public Microfacet
        {
        public:
            explicit MicrofacetReflection(std::unique_ptr<GGXDistribution> distribution, std::unique_ptr<Fresnel> fresnel);

            std::pair<glm::vec3, glm::vec3> sampleWi(float roughness, const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler) const override;
            glm::vec3 getBsdf(float roughness, const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent) const override;
            float getPdf(float roughness, const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent) const override;

        private:
            std::unique_ptr<GGXDistribution> m_microfacet;
            std::unique_ptr<Fresnel> m_fresnel;

        public:
            friend class KullaConty;
        };
    }
}

#endif
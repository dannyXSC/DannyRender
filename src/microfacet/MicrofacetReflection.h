#ifndef __DANNY__MICROFACET__MICROFACETREFLECTION__
#define __DANNY__MICROFACET__MICROFACETREFLECTION__

#include <utility>
#include <memory>

#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

#include <core/Sampler.hpp>

namespace danny
{
    namespace microfacet
    {
        template <typename MicrofacetDistribution, bool tSampleVisibleNormals = true>
        class MicrofacetReflection
        {
        public:
            explicit MicrofacetReflection(float roughness);

            std::pair<glm::vec3, glm::vec3> sampleWi(const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler, float nt_over_ni) const;
            // std::pair<glm::vec3, glm::vec3> sampleWi(const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler, const glm::vec3 &nt_over_ni, const glm::vec3 &kt_over_ki) const;
            glm::vec3 getBsdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, float nt_over_ni) const;
            // glm::vec3 getBsdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const glm::vec3 &nt_over_ni, const glm::vec3 &kt_over_ki) const;
            float getPdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent) const;

        private:
            MicrofacetDistribution m_microfacet;
        };
    }
}

#include "MicrofacetReflection.inl"
#endif
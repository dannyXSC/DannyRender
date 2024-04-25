#include "MicrofacetReflection.h"

#include <core/Math.hpp>
#include <microfacet/Fresnel.h>
#include <microfacet/GGXDistribution.h>

namespace danny
{
    namespace microfacet
    {
        MicrofacetReflection::MicrofacetReflection(std::unique_ptr<GGXDistribution> distribution, std::unique_ptr<Fresnel> fresnel)
            : m_microfacet(std::move(distribution)), m_fresnel(std::move(fresnel))
        {
        }

        std::pair<glm::vec3, glm::vec3> MicrofacetReflection::sampleWi(float roughness,
                                                                       const glm::vec3 &wo_tangent,
                                                                       std::shared_ptr<core::UniformSampler> sampler) const
        {
            // 判断方法
            glm::vec3 wh_tangent = m_microfacet->sample(roughness, sampler);

            glm::vec3 wi_tangent = 2 * glm::dot(wo_tangent, wh_tangent) * wh_tangent - wo_tangent;

            float cos_o_h = glm::dot(wo_tangent, wh_tangent);

            glm::vec3 f(0.f);

            auto F = m_fresnel->evaluate(glm::abs(cos_o_h));
            auto G = m_microfacet->g1(roughness, wo_tangent, wh_tangent) * m_microfacet->g1(roughness, wi_tangent, wh_tangent);
            f = F * G * glm::abs(cos_o_h / (core::math::cosTheta(wo_tangent) * core::math::cosTheta(wh_tangent)));

            return std::make_pair(wi_tangent, f);
        }

        glm::vec3 MicrofacetReflection::getBsdf(float roughness, const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent) const
        {
            auto wh_tangent = glm::normalize(wi_tangent + wo_tangent);
            auto F = m_fresnel->evaluate(glm::abs(glm::dot(wo_tangent, wh_tangent)));
            auto D = m_microfacet->d(roughness, wh_tangent);
            auto G = m_microfacet->g1(roughness, wo_tangent, wh_tangent) * m_microfacet->g1(roughness, wi_tangent, wh_tangent);

            glm::vec3 bsdf = F * D * G / glm::abs(4.f * core::math::cosTheta(wi_tangent) * core::math::cosTheta(wo_tangent));
            return bsdf;
        }

        float MicrofacetReflection::getPdf(float roughness, const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent) const
        {
            // wo的概率密度，由wh的概率密度推出
            auto wh_tangent = glm::normalize(wi_tangent + wo_tangent);

            return glm::abs(m_microfacet->pdf(roughness, wh_tangent) / (4.f * glm::dot(wo_tangent, wh_tangent)));
        }
    }
}
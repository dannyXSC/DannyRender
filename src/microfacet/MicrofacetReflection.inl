#include "MicrofacetReflection.h"

#include <microfacet/Fresnel.h>
#include <core/Math.hpp>
namespace danny
{
    namespace microfacet
    {
        template <typename MicrofacetDistribution, bool tSampleVisibleNormals>
        MicrofacetReflection<MicrofacetDistribution, tSampleVisibleNormals>::MicrofacetReflection(float roughness)
            : m_microfacet(roughness)
        {
        }

        template <typename MicrofacetDistribution, bool tSampleVisibleNormals>
        std::pair<glm::vec3, glm::vec3> MicrofacetReflection<MicrofacetDistribution, tSampleVisibleNormals>::
            sampleWi(const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler, float nt_over_ni) const
        {
            // 判断方法
            glm::vec3 wh_tangent = m_microfacet.sampleWhWmlt07(sampler);

            glm::vec3 wi_tangent = 2 * glm::dot(wo_tangent, wh_tangent) * wh_tangent - wo_tangent;

            float cos_o_h = glm::dot(wo_tangent, wh_tangent);

            float f = 0.f;

            auto F = fresnel::Dielectric()(nt_over_ni, glm::abs(cos_o_h));
            auto G = m_microfacet.g1(wo_tangent, wh_tangent) * m_microfacet.g1(wi_tangent, wh_tangent);
            f = F * G * glm::abs(cos_o_h / (core::math::cosTheta(wo_tangent) * core::math::cosTheta(wh_tangent)));

            return std::make_pair(wi_tangent, glm::vec3(f));
        }

        template <typename MicrofacetDistribution, bool tSampleVisibleNormals>
        glm::vec3 MicrofacetReflection<MicrofacetDistribution, tSampleVisibleNormals>::
            getBsdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, float nt_over_ni) const
        {
            auto wh_tangent = glm::normalize(wi_tangent + wo_tangent);
            auto F = fresnel::Dielectric()(nt_over_ni, glm::abs(glm::dot(wo_tangent, wh_tangent)));
            auto D = m_microfacet.d(wh_tangent);
            auto G = m_microfacet.g1(wo_tangent, wh_tangent) * m_microfacet.g1(wi_tangent, wh_tangent);

            float bsdf = F * D * G / glm::abs(4.f * core::math::cosTheta(wi_tangent) * core::math::cosTheta(wo_tangent));
            return glm::vec3(bsdf);
        }

        template <typename MicrofacetDistribution, bool tSampleVisibleNormals>
        float MicrofacetReflection<MicrofacetDistribution, tSampleVisibleNormals>::
            getPdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent) const
        {
            // wo的概率密度，由wh的概率密度推出
            auto wh_tangent = glm::normalize(wi_tangent + wo_tangent);

            return glm::abs(m_microfacet.pdfWmlt07(wh_tangent) / (4.f * glm::dot(wo_tangent, wh_tangent)));
        }
    }
}
#include "Metal.h"

namespace danny
{
    namespace material
    {
        Metal::Metal(std::shared_ptr<texture::Texture> roughness, float ior)
            : m_roughness(roughness), m_ior(ior)
        {
        }

        std::pair<glm::vec3, glm::vec3> Metal::sampleWi(const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler,
                                                        const geometry::Intersection &intersection) const
        {
            microfacet::MicrofacetReflection<microfacet::GGXDistribution> microfacet_reflection(m_roughness->fetch(intersection).x);
            return microfacet_reflection.sampleWi(wo_tangent, sampler, m_ior);
        }

        glm::vec3 Metal::getBsdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const
        {
            microfacet::MicrofacetReflection<microfacet::GGXDistribution> microfacet_reflection(m_roughness->fetch(intersection).x);
            return microfacet_reflection.getBsdf(wi_tangent, wo_tangent, m_ior);
        }

        float Metal::getPdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const
        {
            microfacet::MicrofacetReflection<microfacet::GGXDistribution> microfacet_reflection(m_roughness->fetch(intersection).x);
            return microfacet_reflection.getPdf(wi_tangent, wo_tangent);
        }

        bool Metal::hasDeltaDistribution(const geometry::Intersection &intersection) const
        {
            return false;
        }

        bool Metal::useMultipleImportanceSampling(const geometry::Intersection &intersection) const
        {
            return true;
        }

        bool Metal::isSpecular(const geometry::Intersection &intersection) const
        {
            return true;
        }
    }
}
#include "Metal.h"

namespace danny
{
    namespace material
    {
        Metal::Xml::Xml(const xml::Node &node)
        {
            fresnel = microfacet::Fresnel::Xml::factory(node.child("Fresnel", true));
            roughness = texture::Texture::Xml::factory(node.child("Roughness", true));
        }

        std::unique_ptr<BsdfMaterial> Metal::Xml::create() const
        {
            return std::make_unique<Metal>(*this);
        }

        Metal::Metal(std::shared_ptr<texture::Texture> roughness, std::unique_ptr<microfacet::Fresnel> fresnel)
            : m_roughness(roughness)
        {
            std::unique_ptr<microfacet::GGXDistribution> distribution = std::make_unique<microfacet::Wmlt07>();
            m_microfacet_reflection = std::make_unique<microfacet::MicrofacetReflection>(std::move(distribution), std::move(fresnel));
        }

        std::pair<glm::vec3, glm::vec3> Metal::sampleWi(const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler, const geometry::Intersection &intersection) const
        {
            auto roughness = m_roughness->fetch(intersection).x;
            return m_microfacet_reflection->sampleWi(roughness, wo_tangent, sampler);
        }

        glm::vec3 Metal::getBsdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const
        {
            auto roughness = m_roughness->fetch(intersection).x;
            return m_microfacet_reflection->getBsdf(roughness, wi_tangent, wo_tangent);
        }

        float Metal::getPdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const
        {
            auto roughness = m_roughness->fetch(intersection).x;
            return m_microfacet_reflection->getPdf(roughness, wi_tangent, wo_tangent);
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
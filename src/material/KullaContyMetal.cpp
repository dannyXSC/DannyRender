#include "KullaContyMetal.h"
#include <iostream>

#include <microfacet/Fresnel.h>
#include <core/Math.hpp>
namespace danny
{
    namespace material
    {
        KullaContyMetal::Xml::Xml(const xml::Node &node)
        {
            roughness = texture::Texture::Xml::factory(node.child("Roughness", true));
            precomputed_e = texture::Texture::Xml::factory(node.child("PrecomputedE", true));
            precomputed_e_avg = texture::Texture::Xml::factory(node.child("PrecomputedEAvg", true));
            node.parseChildText("N", &n.x, &n.y, &n.z);
            node.parseChildText("K", &k.x, &k.y, &k.z);
        }

        std::unique_ptr<BsdfMaterial> KullaContyMetal::Xml::create() const
        {
            return std::make_unique<KullaContyMetal>(*this);
        }

        KullaContyMetal::KullaContyMetal(std::shared_ptr<texture::Texture> roughness,
                                         std::unique_ptr<texture::Texture> precomputed_e,
                                         std::unique_ptr<texture::Texture> precomputed_e_avg,
                                         const glm::vec3 &n, const glm::vec3 &k)
            : Metal(std::move(roughness), std::make_unique<microfacet::Conductor>(n, k))
        {
            auto fresnel = std::make_unique<microfacet::Conductor>(n, k);
            std::unique_ptr<microfacet::GGXDistribution> distribution = std::make_unique<microfacet::Wmlt07>();
            auto microfacet = std::make_unique<microfacet::MicrofacetReflection>(std::move(distribution), std::move(fresnel));

            auto [r, g] = microfacet::Gulbrandsen::nk2rg(n, k);
            kullaconty = std::make_unique<microfacet::KullaConty>(std::move(microfacet),
                                                                  std::move(precomputed_e),
                                                                  std::move(precomputed_e_avg),
                                                                  r, g);
        }

        std::pair<glm::vec3, glm::vec3> KullaContyMetal::sampleWi(const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler,
                                                                  const geometry::Intersection &intersection) const
        {
            auto roughness = m_roughness->fetch(intersection).x;
            auto [wi_tangent, f] = Metal::sampleWi(wo_tangent, sampler, intersection);
            float pdf = Metal::getPdf(wi_tangent, wo_tangent, intersection);
            auto kc = kullaconty->getBsdf(roughness, wi_tangent, wo_tangent) * core::math::cosTheta(wi_tangent) / pdf;
            auto kc_sum = kc.x + kc.y + kc.z;
            if (kc_sum <= 0.f || glm::isinf(kc_sum))
            {
                kc = glm::vec3(0.f);
            }
            f += kc;
            return std::make_pair(wi_tangent, f);
        }

        glm::vec3 KullaContyMetal::getBsdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const
        {
            auto roughness = m_roughness->fetch(intersection).x;
            return Metal::getBsdf(wi_tangent, wo_tangent, intersection) + kullaconty->getBsdf(roughness, wi_tangent, wo_tangent);
        }
    }
}

#include "KullaContyReflection.h"

#include <iostream>

#include <geometry/Intersection.hpp>
#include <core/Math.hpp>
#include <microfacet/MicrofacetReflection.h>
#include <microfacet/Fresnel.h>
#include <texture/Texture.h>
#include <microfacet/GGXDistribution.h>

namespace danny
{
    namespace microfacet
    {
        KullaConty::KullaConty(std::unique_ptr<microfacet::MicrofacetReflection> microfacet,
                               std::unique_ptr<texture::Texture> precomputed_e,
                               std::unique_ptr<texture::Texture> precomputed_e_avg,
                               const glm::vec3 &r, const glm::vec3 &g)
            : m_microfacet(std::move(microfacet)),
              m_precomputed_e(std::move(precomputed_e)),
              m_precomputed_e_avg(std::move(precomputed_e_avg))
        {
            Gulbrandsen eval(r, g);
            m_fresnel_avg = eval.evaluateAvg();
        }

        glm::vec3 KullaConty::getBsdf(float roughness, const glm::vec3 &wi_tangent,
                                      const glm::vec3 &wo_tangent) const
        {
            geometry::Intersection inter;

            inter.uv = glm::vec2(glm::abs(core::math::cosTheta(wi_tangent)), roughness);
            glm::vec3 Ei = m_precomputed_e->fetch(inter);
            inter.uv = glm::vec2(glm::abs(core::math::cosTheta(wo_tangent)), roughness);
            glm::vec3 Eo = m_precomputed_e->fetch(inter);
            glm::vec3 E_avg = m_precomputed_e_avg->fetch(inter);

            auto wh_tangent = glm::normalize(wi_tangent + wo_tangent);
            glm::vec3 fresnel = m_microfacet->m_fresnel->evaluate(glm::abs(glm::dot(wo_tangent, wh_tangent)));

            glm::vec3 fresnel_avg = m_fresnel_avg;

            glm::vec3 f = (1.f - Ei) * (1.f - Eo) / ((1.f - E_avg) * glm::vec3(M_PI));
            glm::vec3 term = fresnel_avg * fresnel_avg * E_avg / (1.f - fresnel_avg * (1.f - E_avg));

            // float pdf = m_microfacet->getPdf(roughness, wi_tangent, wo_tangent);
            return f * term;
        }

    }
}
#include "GGXDistribution.h"

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include <core/Math.hpp>
#include <core/Sampler.hpp>
#include <core/CoordinateSpace.h>
#include <geometry/SphericalCoordination.hpp>

namespace danny
{
    namespace microfacet
    {
        GGXDistribution::GGXDistribution(float roughness)
            : m_ag(roughness)
        {
        }
        glm::vec3 GGXDistribution::sampleWhWmlt07(std::shared_ptr<core::UniformSampler> sampler) const
        {
            float epsilon_1 = sampler->sample(), epsilon_2 = sampler->sample();
            float theta_m = glm::atan(m_ag * glm::sqrt(epsilon_1) / glm::sqrt(1.0f - epsilon_1));
            float phi_m = 2 * M_PI * epsilon_2;

            return geometry::SphericalCoordinate(1.f, theta_m, phi_m).toCartesianCoordinate();
        }
        float GGXDistribution::pdfWmlt07(const glm::vec3 &wh_tangent) const
        {
            return glm::abs(d(wh_tangent) * core::math::cosTheta(wh_tangent));
        }
        float GGXDistribution::d(const glm::vec3 &wh_tangent) const
        {
            auto costheta_h = core::math::cosTheta(wh_tangent);
            if (costheta_h > 0.0f)
            {
                auto temp = m_ag * m_ag + core::math::tan2Theta(wh_tangent);
                return m_ag * m_ag / (glm::pi<float>() * costheta_h * costheta_h * costheta_h * costheta_h * temp * temp);
            }

            return 0.0f;
        }

        float GGXDistribution::g1(const glm::vec3 &wv_tangent, const glm::vec3 &wh_tangent) const
        {
            if (glm::dot(wv_tangent, wh_tangent) / core::math::cosTheta(wv_tangent) > 0.0f)
            {
                return 2.0f / (1.0f + glm::sqrt(1.0f + m_ag * m_ag * core::math::tan2Theta(wv_tangent)));
            }

            return 0.0f;
        }
    }
}
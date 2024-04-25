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
        float GGXDistribution::d(float roughness, const glm::vec3 &wh_tangent) const
        {
            auto costheta_h = core::math::cosTheta(wh_tangent);
            if (costheta_h > 0.0f)
            {
                auto temp = roughness * roughness + core::math::tan2Theta(wh_tangent);
                return roughness * roughness / (glm::pi<float>() * costheta_h * costheta_h * costheta_h * costheta_h * temp * temp);
            }

            return 0.0f;
        }

        float GGXDistribution::g1(float roughness, const glm::vec3 &wv_tangent, const glm::vec3 &wh_tangent) const
        {
            if (glm::dot(wv_tangent, wh_tangent) / core::math::cosTheta(wv_tangent) > 0.0f)
            {
                return 2.0f / (1.0f + glm::sqrt(1.0f + roughness * roughness * core::math::tan2Theta(wv_tangent)));
            }

            return 0.0f;
        }

        glm::vec3 Wmlt07::sample(float roughness, std::shared_ptr<core::UniformSampler> sampler) const
        {
            float epsilon_1 = sampler->sample(), epsilon_2 = sampler->sample();
            float theta_m = glm::atan(roughness * glm::sqrt(epsilon_1) / glm::sqrt(1.0f - epsilon_1));
            float phi_m = 2 * M_PI * epsilon_2;

            return geometry::SphericalCoordinate(1.f, theta_m, phi_m).toCartesianCoordinate();
        }
        float Wmlt07::pdf(float roughness, const glm::vec3 &wh_tangent) const
        {
            return glm::abs(d(roughness, wh_tangent) * core::math::cosTheta(wh_tangent));
        }
    }
}
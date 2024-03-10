#include "Diffuse.hpp"
#include <iostream>

#include <glm/gtc/constants.hpp>

#include <core/Math.hpp>

namespace danny
{
    namespace material
    {
        Diffuse::Diffuse(std::shared_ptr<texture::Texture> texture)
            : m_texture(texture)
        {
            // texture
        }
        std::pair<glm::vec3, glm::vec3> Diffuse::sampleWi(const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler,
                                                          const geometry::Intersection &intersection) const
        {
            glm::vec3 wi_tangent(0.f);
            glm::vec3 f(0.f);

            if (core::math::cosTheta(wo_tangent) > 0.f)
            {
                wi_tangent = core::math::sampleHemisphereCosine(sampler->sample(), sampler->sample()).toCartesianCoordinate();
                f = m_texture->fetch(intersection);
            }

            return std::make_pair(wi_tangent, f);
        }
        glm::vec3 Diffuse::getBsdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const
        {
            if (core::math::cosTheta(wi_tangent) > 0.f && core::math::cosTheta(wo_tangent) > 0.0f)
            {
                return m_texture->fetch(intersection) * glm::one_over_pi<float>();
            }
            else
            {
                return glm::vec3(0.f);
            }
        }
        float Diffuse::getPdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const
        {
            if (core::math::cosTheta(wi_tangent) > 0.f && core::math::cosTheta(wo_tangent) > 0.0f)
            {
                return core::math::cosTheta(wi_tangent) * glm::one_over_pi<float>();
            }
            else
            {
                return 0.f;
            }
        }
        bool Diffuse::hasDeltaDistribution(const geometry::Intersection &intersection) const
        {
            return false;
        }
        bool Diffuse::useMultipleImportanceSampling(const geometry::Intersection &intersection) const
        {
            return false;
        }
        bool Diffuse::isSpecular(const geometry::Intersection &intersection) const
        {
            return false;
        }
    }
}
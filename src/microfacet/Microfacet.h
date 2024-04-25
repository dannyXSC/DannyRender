#ifndef __DANNY__MICROFACET__MICROFACET__
#define __DANNY__MICROFACET__MICROFACET__

#include <memory>

#include <glm/vec3.hpp>

#include <core/Sampler.hpp>

namespace danny
{
    namespace microfacet
    {
        class Microfacet
        {
        public:
            virtual ~Microfacet() = default;
            virtual std::pair<glm::vec3, glm::vec3> sampleWi(float roughness, const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler) const = 0;
            virtual glm::vec3 getBsdf(float roughness, const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent) const = 0;
            virtual float getPdf(float roughness, const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent) const = 0;
        };
    }
}

#endif
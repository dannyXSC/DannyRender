#ifndef __DANNY__MICROFACET__GGXDISTRIBUTION__
#define __DANNY__MICROFACET__GGXDISTRIBUTION__
#include <core/forward_decl.h>

#include <memory>

#include <glm/vec3.hpp>

namespace danny
{
    namespace microfacet
    {
        class GGXDistribution
        {
        public:
            virtual ~GGXDistribution() = default;
            // Samples from distribution of normals.
            virtual glm::vec3 sample(float roughness, std::shared_ptr<core::UniformSampler> sampler) const = 0;
            virtual float pdf(float roughness, const glm::vec3 &wh_tangent) const = 0;
            // Distribution and Masking-Shadowing functions.
            float d(float roughness, const glm::vec3 &wh_tangent) const;
            float g1(float roughness, const glm::vec3 &wv_tangent, const glm::vec3 &wh_tangent) const;
        };

        class Wmlt07 : public GGXDistribution
        {
        public:
            glm::vec3 sample(float roughness, std::shared_ptr<core::UniformSampler> sampler) const override;
            float pdf(float roughness, const glm::vec3 &wh_tangent) const override;
        };
    }
}

#endif
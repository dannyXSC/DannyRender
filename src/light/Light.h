#ifndef __DANNY__LIGHT__LIGHT__
#define __DANNY__LIGHT__LIGHT__

#include <memory>

#include <glm/vec3.hpp>

#include <core/Sampler.hpp>

namespace danny
{
    namespace light
    {
        struct LightSample
        {
            glm::vec3 wi_world;
            glm::vec3 le{0.0f, 0.0f, 0.0f};
            float pdf_w{0.0f};
            float distance;
        };

        class Light
        {
        public:
            virtual ~Light() = default;

            // virtual Photon castPhoton(core::UniformSampler &sampler) const = 0;
            virtual LightSample sample(std::shared_ptr<core::UniformSampler> sampler, const geometry::Intersection &intersection) const = 0;
            virtual LightSample getVisibleSample(const core::Scene &scene, const geometry::Ray &ray) const = 0;
            virtual glm::vec3 getLe(const glm::vec3 &wi_world, const glm::vec3 &light_plane_normal, float distance) const = 0;
            virtual float getPdf(const glm::vec3 &wi_world, const glm::vec3 &light_plane_normal, float distance) const = 0;
            virtual bool hasDeltaDistribution() const = 0;
            virtual std::shared_ptr<geometry::Object> getObject() const = 0;
        };
    }
}

#endif
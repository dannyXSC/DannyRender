#ifndef __DANNY__LIGHT__DIFFUSEAREALIGHT__
#define __DANNY__LIGHT__DIFFUSEAREALIGHT__

#include <memory>

#include <glm/vec3.hpp>

#include <light/Light.h>
#include <core/Sampler.hpp>
#include <geometry/Object.hpp>
#include <geometry/Intersection.hpp>
#include <core/Scene.h>

namespace danny
{
    namespace light
    {
        class DiffuseArealight : public Light
        {
        public:
            explicit DiffuseArealight(const glm::vec3 &flux, std::shared_ptr<geometry::Object> obj);

            // Photon castPhoton(core::UniformSampler &sampler) const override;
            LightSample sample(std::shared_ptr<core::UniformSampler> sampler, const geometry::Intersection &intersection) const override;
            LightSample getVisibleSample(const core::Scene &scene, const geometry::Ray &ray) const override;
            glm::vec3 getLe(const glm::vec3 &wi_world, const glm::vec3 &light_plane_normal, float distance) const override;
            float getPdf(const glm::vec3 &wi_world, const glm::vec3 &light_plane_normal, float distance) const override;
            bool hasDeltaDistribution() const override;
            std::shared_ptr<geometry::Object> getObject() const override;

        private:
            glm::vec3 m_flux;
            std::shared_ptr<geometry::Object> m_object;
            glm::vec3 m_le;
            float m_pdf;
        };
    }
}

#endif
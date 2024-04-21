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
            // Xml structure of the class.
            struct Xml : public Light::Xml
            {
                glm::vec3 flux;
                std::unique_ptr<geometry::Object::Xml> object;

                explicit Xml(const xml::Node &node);
                std::unique_ptr<Light> create() const override;
            };

        public:
            explicit DiffuseArealight(const DiffuseArealight::Xml &xml)
                : DiffuseArealight(xml.flux, xml.object->create()){};
            explicit DiffuseArealight(const glm::vec3 &flux, std::unique_ptr<geometry::Object> obj);

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
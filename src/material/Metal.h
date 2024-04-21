#ifndef __DANNY__MATERIAL__METAL__
#define __DANNY__MATERIAL__METAL__

#include <memory>

#include <material/BsdfMaterial.hpp>
#include <microfacet/MicrofacetReflection.h>
#include <microfacet/GGXDistribution.h>
#include <texture/Texture.h>

namespace danny
{
    namespace material
    {
        class Metal : public BsdfMaterial
        {
        public:
            // Xml structure of the class.
            struct Xml : public BsdfMaterial::Xml
            {
                float ior;
                // glm::vec3 ior_n;
                // glm::vec3 ior_k;
                std::unique_ptr<texture::Texture::Xml> roughness;

                explicit Xml(const xml::Node &node);
                Xml(const float &p_ior, std::unique_ptr<texture::Texture::Xml> p_roughness);
                // Xml(const glm::vec3 &p_ior_n, const glm::vec3 &p_ior_k, std::unique_ptr<texture::Texture::Xml> p_roughness);
                std::unique_ptr<BsdfMaterial> create() const override;
            };

        public:
            explicit Metal(const Metal::Xml &xml)
                : Metal(std::move(xml.roughness->create()), xml.ior){};
            explicit Metal(std::shared_ptr<texture::Texture> roughness, float ior);

            // pair.first is sampled wi
            // pair.second is the corresponding f (all of BSDF except for the L, fr * cos / pdf)
            std::pair<glm::vec3, glm::vec3> sampleWi(const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler,
                                                     const geometry::Intersection &intersection) const override;
            // prepare bsdf for samplewi.second
            glm::vec3 getBsdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const override;
            float getPdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const override;
            // 需不需要进行重要性采样
            bool hasDeltaDistribution(const geometry::Intersection &intersection) const override;
            bool useMultipleImportanceSampling(const geometry::Intersection &intersection) const override;
            bool isSpecular(const geometry::Intersection &intersection) const override;

        private:
            std::shared_ptr<texture::Texture> m_roughness;
            float m_ior;
        };
    }
}

#endif
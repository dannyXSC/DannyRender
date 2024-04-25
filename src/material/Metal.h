#ifndef __DANNY__MATERIAL__METAL__
#define __DANNY__MATERIAL__METAL__

#include <memory>

#include <core/forward_decl.h>
#include <material/BsdfMaterial.hpp>
#include <microfacet/Fresnel.h>
#include <microfacet/GGXDistribution.h>
#include <texture/Texture.h>
#include <microfacet/MicrofacetReflection.h>

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
                std::unique_ptr<texture::Texture::Xml> roughness;
                std::unique_ptr<microfacet::Fresnel::Xml> fresnel;

                explicit Xml(const xml::Node &node);
                std::unique_ptr<BsdfMaterial> create() const override;
            };

        public:
            explicit Metal(const Metal::Xml &xml)
                : Metal(std::move(xml.roughness->create()), std::move(xml.fresnel->create())){};
            explicit Metal(std::shared_ptr<texture::Texture> roughness,
                           std::unique_ptr<microfacet::Fresnel> fresnel);

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

        protected:
            std::shared_ptr<texture::Texture> m_roughness;
            std::unique_ptr<microfacet::MicrofacetReflection> m_microfacet_reflection;
        };
    }
}

#endif
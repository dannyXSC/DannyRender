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
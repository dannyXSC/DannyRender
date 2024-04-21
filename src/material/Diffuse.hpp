#ifndef __DANNY__MATERIAL__DIFFUSE__
#define __DANNY__MATERIAL__DIFFUSE__

#include <memory>

#include <material/BsdfMaterial.hpp>
#include <texture/Texture.h>
#include <texture/ConstantTexture.h>

namespace danny
{
    namespace material
    {
        class Diffuse : public BsdfMaterial
        {
        public:
            // Xml structure of the class.
            struct Xml : public BsdfMaterial::Xml
            {
                std::unique_ptr<texture::Texture::Xml> kd;

                explicit Xml(const xml::Node &node);
                explicit Xml(std::unique_ptr<texture::Texture::Xml> p_kd);
                std::unique_ptr<BsdfMaterial> create() const override;
            };

        public:
            explicit Diffuse(const Diffuse::Xml &xml) : Diffuse(xml.kd->create()){};
            explicit Diffuse(std::shared_ptr<texture::Texture> texture);

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
            std::shared_ptr<texture::Texture> m_texture;
        };
    }
}

#endif
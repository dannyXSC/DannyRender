#ifndef __DANNY__MATERIAL__KULLACONTYMETAL__
#define __DANNY__MATERIAL__KULLACONTYMETAL__

#include <material/Metal.h>
#include <microfacet/KullaContyReflection.h>

namespace danny
{
    namespace material
    {
        class KullaContyMetal : public Metal
        {
        public:
            // Xml structure of the class.
            struct Xml : public BsdfMaterial::Xml
            {
                std::unique_ptr<texture::Texture::Xml> roughness;
                std::unique_ptr<texture::Texture::Xml> precomputed_e;
                std::unique_ptr<texture::Texture::Xml> precomputed_e_avg;
                glm::vec3 n, k;

                explicit Xml(const xml::Node &node);
                std::unique_ptr<BsdfMaterial> create() const override;
            };

        public:
            // only for conductor
            // can not use other fresnel
            explicit KullaContyMetal(const KullaContyMetal::Xml &xml)
                : KullaContyMetal(xml.roughness->create(),
                                  xml.precomputed_e->create(),
                                  xml.precomputed_e_avg->create(),
                                  xml.n, xml.k){};
            // only for conductor
            // can not use other fresnel
            explicit KullaContyMetal(std::shared_ptr<texture::Texture> roughness,
                                     std::unique_ptr<texture::Texture> precomputed_e,
                                     std::unique_ptr<texture::Texture> precomputed_e_avg,
                                     const glm::vec3 &n, const glm::vec3 &k);

            // pair.first is sampled wi
            // pair.second is the corresponding f (all of BSDF except for the L, fr * cos / pdf)
            std::pair<glm::vec3, glm::vec3> sampleWi(const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler,
                                                     const geometry::Intersection &intersection) const override;
            // prepare bsdf for samplewi.second
            glm::vec3 getBsdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const override;

        private:
            std::unique_ptr<microfacet::KullaConty> kullaconty;
        };
    }
}
#endif
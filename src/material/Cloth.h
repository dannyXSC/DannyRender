#ifndef __DANNY__MATERIAL__CLOTH__
#define __DANNY__MATERIAL__CLOTH__
#include <core/forward_decl.h>
#include <material/BsdfMaterial.hpp>
#include <microfacet/MicrocylinderCloth.h>

#include <memory>

namespace danny
{
    namespace material
    {
        struct ClothPara
        {
            glm::vec3 albedo;
            float gamma_s;
            float gamma_v;
            float k_d;
            float eta;
            float alpha;
            // len(tangent_offsets) == sample_num
            std::vector<float> tangent_offsets;
        };
        class Cloth : public BsdfMaterial
        {
        public:
            explicit Cloth(const ClothPara &cloth_para1, const ClothPara &cloth_para2);

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
            ClothPara cloth_para1, cloth_para2;
            std::unique_ptr<danny::microfacet::MicrocylinderCloth> micro1, micro2;
        };
    }
}

#endif

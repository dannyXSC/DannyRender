#ifndef __DANNY__MATERIAL__BSDFMATERIAL__
#define __DANNY__MATERIAL__BSDFMATERIAL__

#include <core/forward_decl.h>
#include <core/Sampler.hpp>
#include <geometry/Intersection.hpp>

#include <xml/Node.h>
#include <glm/vec3.hpp>
#include <utility>

namespace danny
{
    namespace material
    {
        class BsdfMaterial
        {
        public:
            // Xml structure of the class.
            struct Xml
            {
                virtual ~Xml() = default;
                virtual std::unique_ptr<BsdfMaterial> create() const = 0;
                static std::unique_ptr<BsdfMaterial::Xml> factory(const xml::Node &node);
            };

        public:
            virtual ~BsdfMaterial() = default;

            // wi, wo should be in tangent space for sampleWo, getBsdf and getPdf.
            // This makes it easier and efficient to calculate some computations.
            // sampleWo returns <wo, f> pair where f = bsdf * cos / pdf

            // wi_tangent indicates the incoming direction of the light&importance transport equation integrand.
            // So, if you are path tracing (radiance transport), wi_tangent is the incoming light direction.
            // If you are light tracing (importance transport), wi_tangent is the incoming importance direction.
            // This also means that wo_tangent is the known direction and wi_tangent is the direction to be sampled.

            // determine choose reflection or refraction
            // pair.first 0 for reflection
            // pair.second is the possibility of the action
            virtual std::pair<int, float> chooseBsdf(const glm::vec3 &wo_tangent, core::RealSampler &sampler,
                                                     const geometry::Intersection &intersection) const { return std::make_pair(0, 1.0f); };
            // pair.first is sampled wi
            // pair.second is the corresponding f (all of BSDF except for the L, fr * cos / pdf)
            virtual std::pair<glm::vec3, glm::vec3> sampleWi(const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler, const geometry::Intersection &intersection) const = 0;
            // prepare bsdf for samplewi.second
            virtual glm::vec3 getBsdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const = 0;
            virtual float getPdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const = 0;
            // 需不需要进行重要性采样
            virtual bool hasDeltaDistribution(const geometry::Intersection &intersection) const = 0;
            virtual bool useMultipleImportanceSampling(const geometry::Intersection &intersection) const = 0;
            virtual bool isSpecular(const geometry::Intersection &intersection) const = 0;
        };
    }
}
#endif
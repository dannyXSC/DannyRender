#include "Cloth.h"
#include <iostream>

#include <core/Math.hpp>
#include <geometry/Transformation.hpp>

namespace danny
{
    namespace material
    {
        Cloth::Cloth(const ClothPara &cloth_para1, const ClothPara &cloth_para2)
            : cloth_para1(cloth_para1), cloth_para2(cloth_para2),
              micro1(std::make_unique<microfacet::MicrocylinderCloth>(cloth_para1)),
              micro2(std::make_unique<microfacet::MicrocylinderCloth>(cloth_para2))
        {
        }
        std::pair<glm::vec3, glm::vec3> Cloth::sampleWi(const glm::vec3 &wo_tangent, std::shared_ptr<core::UniformSampler> sampler,
                                                        const geometry::Intersection &intersection) const
        {
            // generate wi uniformly
            // bsdf
            glm::vec3 wi_tangent(0.f);
            glm::vec3 f(0.f);

            if (core::math::cosTheta(wo_tangent) > 0.f)
            {
                wi_tangent = core::math::sampleHemisphereUniform(sampler->sample(), sampler->sample()).toCartesianCoordinate();
                f = getBsdf(wi_tangent, wo_tangent, intersection);
            }

            return std::make_pair(wi_tangent, f);
        }

        glm::vec3 Cloth::getBsdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const
        {
            int sample_num1 = cloth_para1.tangent_offsets.size();
            int sample_num2 = cloth_para2.tangent_offsets.size();

            auto u_tangent = glm::normalize(intersection.dpdu_local);
            auto v_tangent = glm::normalize(intersection.dpdv_local);
            auto n_tangent = glm::vec3(0, 0, 1);

            glm::vec3 u_value(0.f), v_value(0.f);
            float Q = 0.f;

            for (int i = 0; i < sample_num1; i++)
            {
                // rotate u tangent_offsets degree with v as the axis
                geometry::Transformation::QuatInfo info;
                info.rotation = glm::vec4(v_tangent.x, v_tangent.y, v_tangent.z, cloth_para1.tangent_offsets[i]);
                geometry::Transformation transfomation_v(info);

                glm::vec3 v = v_tangent;
                glm::vec3 u = transfomation_v.vectorToWorldSpace(u_tangent);
                glm::vec3 n = transfomation_v.vectorToWorldSpace(n_tangent);
                auto [bsdf, p_value] = micro1->getBsdf(u, v, n, wi_tangent, wo_tangent);
                u_value += bsdf / glm::vec3(sample_num1);

                Q += cloth_para1.alpha * p_value / sample_num1;
            }

            // the second thread is perpendicular to the first
            for (int i = 0; i < sample_num2; i++)
            {
                // rotate u tangent_offsets degree with v as the axis
                geometry::Transformation::QuatInfo info;
                info.rotation = glm::vec4(u_tangent.x, u_tangent.y, u_tangent.z, cloth_para2.tangent_offsets[i]);
                geometry::Transformation transfomation_u(info);

                glm::vec3 v = u_tangent;
                // v is the dircetion of the second thread
                glm::vec3 u = transfomation_u.vectorToWorldSpace(v);
                glm::vec3 n = transfomation_u.vectorToWorldSpace(n_tangent);
                auto [bsdf, p_value] = micro1->getBsdf(u, v, n, wi_tangent, wo_tangent);
                v_value += bsdf / glm::vec3(sample_num2);

                Q += cloth_para2.alpha * p_value / sample_num2;
            }

            Q += (1 - cloth_para1.alpha - cloth_para2.alpha) * glm::dot(wo_tangent, n_tangent);
            // std::cout << "bsdf_u: " << glm::to_string(bsdf) << std::endl;

            return (cloth_para1.alpha * u_value + cloth_para2.alpha * v_value) / Q;
        }

        float Cloth::getPdf(const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent, const geometry::Intersection &intersection) const
        {
            if (core::math::cosTheta(wi_tangent) > 0.f && core::math::cosTheta(wo_tangent) > 0.0f)
            {
                // uniform 1/(2*pi)
                return 0.5 * glm ::one_over_pi<float>();
            }
            else
            {
                return 0.f;
            }
        }
        bool Cloth::hasDeltaDistribution(const geometry::Intersection &intersection) const
        {
            return false;
        }
        bool Cloth::useMultipleImportanceSampling(const geometry::Intersection &intersection) const
        {
            return false;
        }
        bool Cloth::isSpecular(const geometry::Intersection &intersection) const
        {
            return false;
        }
    }
}
#ifndef __DANNY__MICROFACET__MICROCYLINDERCLOTH__
#define __DANNY__MICROFACET__MICROCYLINDERCLOTH__
#include <core/forward_decl.h>

#include <material/Cloth.h>
#include <glm/vec3.hpp>
#include <utility>

namespace danny
{
    namespace microfacet
    {
        class MicrocylinderCloth
        {
            /**
             * implement of A Practical Microcylinder Appearance Model for Cloth Rendering
             *https://escholarship.org/content/qt6v11p5b0/qt6v11p5b0_noSplash_21f194c359ebb735072832a2863993e6.pdf
             * refer to https://github.com/kinakomoti-321/Takenoko-Shader/blob/dcd63aaf652a86fc26f6048ce4ab8ee190ac54e9/StandardShader/TakenokoCloth.cginc#L9
             */
        public:
            explicit MicrocylinderCloth(const danny::material::ClothPara &para);
            /**
             * calculate bsdf for a single thread of single direction
             * u: the direction of the tangent curve
             * v: u x n
             * n: normal of the tangent face
             * return: bsdf(all but Li), p_value(for calculate Q)
             */
            std::pair<glm::vec3, float> getBsdf(const glm::vec3 &u, const glm::vec3 &v, const glm::vec3 &n,
                                                const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent) const;

        private:
            glm::vec3 albedo;
            float gamma_s;
            float gamma_v;
            float k_d;
            float eta;

            glm::vec3 getBsdf(float theta_d, float theta_h, float phi_d, float cosThetaI, float cosThetaO) const;
            float scatteringRs(float phi_d, float theta_h) const;
            float scatteringRv(float theta_h, float cosThetaI, float cosThetaO) const;
            float getM(float cos_phi_i, float cos_phi_o, float phi_d) const;
            float getP(float cos_psi_i, float cos_psi_o, float psi_d) const;

        private:
            // should be placed in microcylinder
            struct GeometryPara
            {
                float theta_d;
                float theta_h;
                float cosThetaI;
                float cosThetaO;

                float cosPhiI;
                float cosPhiO;
                float phi_d;

                float cosPsiI;
                float cosPsiO;
                float psi_d;
            };
            MicrocylinderCloth::GeometryPara getPara(const glm::vec3 &u, const glm::vec3 &v, const glm::vec3 &n,
                                                     const glm::vec3 &wi, const glm::vec3 &wo) const;
        };
    }
}
#endif

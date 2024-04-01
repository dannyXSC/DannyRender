#include "MicrocylinderCloth.h"
#include <core/Math.hpp>
#include <microfacet/Fresnel.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include "glm/gtx/string_cast.hpp"

#include <glm/glm.hpp>

namespace danny
{
    namespace microfacet
    {
        MicrocylinderCloth::MicrocylinderCloth(const danny::material::ClothPara &para)
            : albedo(para.albedo), gamma_s(glm::radians(para.gamma_s)), gamma_v(glm::radians(para.gamma_v)), k_d(para.k_d), eta(para.eta)
        {
        }

        std::pair<glm::vec3, float> MicrocylinderCloth::getBsdf(const glm::vec3 &u, const glm::vec3 &v, const glm::vec3 &n,
                                                                const glm::vec3 &wi_tangent, const glm::vec3 &wo_tangent) const
        {
            MicrocylinderCloth::GeometryPara para = getPara(u, v, n, wi_tangent, wo_tangent);
            glm::vec3 fr = getBsdf(para.theta_d, para.theta_h, para.phi_d, para.cosThetaI, para.cosThetaO);
            float m_value = getM(para.cosPhiI, para.cosPhiO, para.phi_d);
            float p_value = getP(para.cosPsiI, para.cosPsiO, para.psi_d);

            glm::vec3 bsdf = p_value * m_value * fr * para.cosThetaI;
            return std::make_pair(bsdf, p_value);
        }
        float MicrocylinderCloth::getM(float cos_phi_i, float cos_phi_o, float phi_d) const
        {
            float m_i = glm::max(cos_phi_i, 0.0f);
            float m_o = glm::max(cos_phi_o, 0.0f);
            // 20 is the setting in the paper
            float u = core::math::unitHeightGaussian(glm::radians(20.f), phi_d);
            float corrated = glm::min(m_i, m_o);
            float uncorrated = m_i * m_o;
            return core::math::lerp(u, uncorrated, corrated);
        }
        float MicrocylinderCloth::getP(float cos_psi_i, float cos_psi_o, float psi_d) const
        {
            float m_i = glm::max(cos_psi_i, 0.0f);
            float m_o = glm::max(cos_psi_o, 0.0f);
            float u = core::math::unitHeightGaussian(glm::radians(20.f), psi_d);
            float corrated = glm::min(m_i, m_o);
            float uncorrated = m_i * m_o;
            return core::math::lerp(u, uncorrated, corrated);
        }

        glm::vec3 MicrocylinderCloth::getBsdf(float theta_d, float theta_h, float phi_d, float cosThetaI, float cosThetaO) const
        {
            float Fr_cosTheta_i = glm::cos(theta_d) * glm::cos(phi_d * 0.5);
            float Fr = fresnel::Dielectric()(eta, Fr_cosTheta_i);
            float Ft = (1.0f - Fr);
            float F = Ft * Ft;

            float rs = Fr * scatteringRs(phi_d, theta_h);
            float rv = F * scatteringRv(theta_h, cosThetaI, cosThetaO);
            return (rs + rv * albedo) / glm::vec3(glm::pow(glm::cos(theta_d), 2.0));
        }

        float MicrocylinderCloth::scatteringRs(float phi_d, float theta_h) const
        {
            return cos(phi_d * 0.5) * core::math::normalizedGaussian(gamma_s, theta_h);
        }
        float MicrocylinderCloth::scatteringRv(float theta_h, float cosThetaI, float cosThetaO) const
        {
            return ((1.0 - k_d) * core::math::normalizedGaussian(gamma_v, theta_h) + k_d) / (cosThetaI + cosThetaO);
        }

        MicrocylinderCloth::GeometryPara MicrocylinderCloth::getPara(const glm::vec3 &u, const glm::vec3 &v, const glm::vec3 &n,
                                                                     const glm::vec3 &wi, const glm::vec3 &wo) const
        {
            float sinThetaI = glm::clamp(glm::dot(wi, u), -1.0f, 1.0f);
            float thetaI = glm::asin(sinThetaI);
            float sinThetaO = glm::clamp(glm::dot(wo, u), -1.0f, 1.0f);
            float thetaO = glm::asin(sinThetaO);
            float theta_h = (thetaI + thetaO) * 0.5;
            float theta_d = (thetaI - thetaO) * 0.5;

            glm::vec3 wi_on_normal = glm::normalize(wi - u * sinThetaI);
            glm::vec3 wo_on_normal = glm::normalize(wo - u * sinThetaO);

            float cosPhiD = glm::clamp(glm::dot(wi_on_normal, wo_on_normal), -1.0f, 1.0f);
            float phi_d = glm::acos(cosPhiD);

            glm::vec3 wi_on_tangent_normal = glm::normalize(wi - v * glm::dot(wi, v));
            glm::vec3 wo_on_tangent_normal = glm::normalize(wo - v * glm::dot(wo, v));

            float cosPsiI = glm::clamp(glm::dot(wi_on_tangent_normal, wo_on_tangent_normal), -1.0f, 1.0f);
            float psi_d = glm::acos(cosPsiI);

            GeometryPara gp;
            gp.theta_d = theta_d;
            gp.theta_h = theta_h;
            gp.cosThetaI = glm::abs(glm::cos(thetaI));
            gp.cosThetaO = glm::abs(glm::cos(thetaO));

            gp.cosPhiI = glm::abs(glm::dot(n, wi_on_normal));
            gp.cosPhiO = glm::abs(glm::dot(n, wo_on_normal));
            gp.phi_d = phi_d;

            // two side rendering
            gp.cosPsiI = glm::abs(glm::dot(n, wi_on_tangent_normal));
            gp.cosPsiO = glm::abs(glm::dot(n, wo_on_tangent_normal));
            gp.psi_d = psi_d;

            return gp;
        }
    }
}
#include "Fresnel.h"

#include <glm/gtc/constants.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

namespace danny
{
    namespace microfacet
    {
        std::unique_ptr<Fresnel::Xml> Fresnel::Xml::factory(const xml::Node &node)
        {
            auto fresnel_type = node.attribute("type", true);

            if (fresnel_type == std::string("Dielectric"))
            {
                return std::make_unique<Dielectric::Xml>(node);
            }
            else if (fresnel_type == std::string("Conductor"))
            {
                return std::make_unique<Conductor::Xml>(node);
            }
            else
            {
                node.throwError("Unknown fresnel type.");
            }

            return nullptr;
        }

        Dielectric::Xml::Xml(const xml::Node &node)
        {
            node.parseChildText("Ior", &ior);
        }

        std::unique_ptr<Fresnel> Dielectric::Xml::create() const
        {
            return std::make_unique<Dielectric>(*this);
        }

        Dielectric::Dielectric(float nt_over_ni) : m_ior(nt_over_ni)
        {
        }

        glm::vec3 Dielectric::evaluate(float costheta) const
        {
            auto g = m_ior * m_ior - 1.0f + costheta * costheta;

            // In the case of total internal reflection, return 1.
            if (g < 0.0f)
            {
                return glm::vec3(1.0f);
            }

            g = glm::sqrt(g);
            auto g_minus_c = g - costheta;
            auto g_plus_c = g + costheta;
            auto temp_div = (costheta * g_plus_c - 1.0f) / (costheta * g_minus_c + 1.0f);

            return glm::vec3(0.5f) * (g_minus_c * g_minus_c) * (1.0f + temp_div * temp_div) / (g_plus_c * g_plus_c);
        }

        Conductor::Xml::Xml(const xml::Node &node)
        {
            node.parseChildText("N", &n.x, &n.y, &n.z);
            node.parseChildText("K", &k.x, &k.y, &k.z);
        }

        std::unique_ptr<Fresnel> Conductor::Xml::create() const
        {
            return std::make_unique<Conductor>(*this);
        }

        Conductor::Conductor(const glm::vec3 &nt_over_ni, const glm::vec3 &kt_over_ki)
            : m_n(nt_over_ni), m_k(kt_over_ki)
        {
        }

        glm::vec3 Conductor::evaluate(float costheta) const
        {
            /*From https://seblagarde.wordpress.com/2013/04/29/memo-on-fresnel-equations*/
            auto cos2theta = costheta * costheta;
            auto sin2theta = 1.0f - cos2theta;
            auto n2 = m_n * m_n;
            auto k2 = m_k * m_k;

            auto t0 = n2 - k2 - sin2theta;
            auto a2_plus_b2 = glm::sqrt(t0 * t0 + 4.0f * n2 * k2);
            auto t1 = a2_plus_b2 + cos2theta;
            auto a = glm::sqrt(0.5f * (a2_plus_b2 + t0));
            auto t2 = 2.0f * a * costheta;
            auto rs = (t1 - t2) / (t1 + t2);

            auto t3 = cos2theta * a2_plus_b2 + sin2theta * sin2theta;
            auto t4 = t2 * sin2theta;
            auto rp = rs * (t3 - t4) / (t3 + t4);

            return 0.5f * (rp + rs);
        }

        Gulbrandsen::Gulbrandsen(const glm::vec3 &r, const glm::vec3 &g)
            : m_r(r), m_g(g), Conductor(rg2nk(r, g).first, rg2nk(r, g).second)
        {
        }
        glm::vec3 Gulbrandsen::evaluateAvg() const
        {
            return glm::vec3(0.087237) + 0.0230685f * m_g - 0.0864902f * m_g * m_g +
                   0.0774594f * m_g * m_g * m_g + 0.782654f * m_r - 0.136432f * m_r * m_r +
                   0.278708f * m_r * m_r * m_r + 0.19744f * m_g * m_r + 0.0360605f * m_g * m_g * m_r -
                   0.2586f * m_g * m_r * m_r;
        }

        std::pair<glm::vec3, glm::vec3> Gulbrandsen::nk2rg(const glm::vec3 &n, const glm::vec3 &k)
        {
            auto k2 = k * k;

            glm::vec3 r = ((n - 1.f) * (n - 1.f) + k2) / ((n + 1.f) * (n + 1.f) + k2);
            auto sqrt_r = glm::sqrt(r);
            auto factor = (1.f + sqrt_r) / (1.f - sqrt_r);
            glm::vec3 g = (factor - n) / (factor - (1.f - r) / (1.f + r));
            return std::make_pair(r, g);
        }
        std::pair<glm::vec3, glm::vec3> Gulbrandsen::rg2nk(const glm::vec3 &r, const glm::vec3 &g)
        {
            auto sqrt_r = glm::sqrt(r);
            glm::vec3 n = g * (1.f - r) / (1.f + r) + (1.f - g) * (1.f + sqrt_r) / (1.f - sqrt_r);
            glm::vec3 k = glm::sqrt((r * ((n + 1.f) * (n + 1.f) - (n - 1.f) * (n - 1.f))) / (1.f - r));
            return std::make_pair(n, k);
        }
    }
}
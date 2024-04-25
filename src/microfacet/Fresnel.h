#ifndef __DANNY__MICROFACET__FRESNEL__
#define __DANNY__MICROFACET__FRESNEL__

#include <glm/vec3.hpp>

#include <xml/Node.h>

namespace danny
{
    namespace microfacet
    {
        class Fresnel
        {
        public:
            // Xml structure of the class.
            struct Xml
            {
                virtual ~Xml() = default;
                virtual std::unique_ptr<Fresnel> create() const = 0;
                static std::unique_ptr<Fresnel::Xml> factory(const xml::Node &node);
            };

        public:
            virtual ~Fresnel() = default;
            virtual glm::vec3 evaluate(float costheta) const = 0;
        };

        class Dielectric : public Fresnel
        {
        public:
            // Xml structure of the class.
            struct Xml : public Fresnel::Xml
            {
                float ior;

                explicit Xml(const xml::Node &node);
                std::unique_ptr<Fresnel> create() const override;
            };

        public:
            explicit Dielectric(const Dielectric::Xml &xml) : Dielectric(xml.ior){};
            explicit Dielectric(float nt_over_ni);
            glm::vec3 evaluate(float costheta) const override;

        private:
            float m_ior;
        };

        class Conductor : public Fresnel
        {
        public:
            // Xml structure of the class.
            struct Xml : public Fresnel::Xml
            {
                glm::vec3 n, k;

                explicit Xml(const xml::Node &node);
                std::unique_ptr<Fresnel> create() const override;
            };

        public:
            explicit Conductor(const Conductor::Xml &xml) : Conductor(xml.n, xml.k){};
            explicit Conductor(const glm::vec3 &nt_over_ni, const glm::vec3 &kt_over_ki);
            glm::vec3 evaluate(float costheta) const override;

        private:
            glm::vec3 m_n;
            glm::vec3 m_k;
        };

        class Gulbrandsen : public Conductor
        {
        public:
            explicit Gulbrandsen(const glm::vec3 &r, const glm::vec3 &g);
            glm::vec3 evaluateAvg() const;

            static std::pair<glm::vec3, glm::vec3> nk2rg(const glm::vec3 &n, const glm::vec3 &k);
            static std::pair<glm::vec3, glm::vec3> rg2nk(const glm::vec3 &r, const glm::vec3 &g);

        private:
            glm::vec3 m_r, m_g;
        };

    }
}

#endif

#ifndef __DANNY__INTEGRATOR__PATHTRACER__
#define __DANNY__INTEGRATOR__PATHTRACER__

#include <iostream>
#include <thread>
#include <mutex>

#include "core/Scene.h"
#include "core/Image.h"

#include <core/forward_decl.h>
#include <xml/Node.h>
#include <geometry/Ray.hpp>

namespace danny
{
    namespace integrator
    {
        constexpr int cPTPatchSize = 16;

        class Pathtracer : public Integrator
        {
        public:
            // Xml structure of the class.
            struct Xml : Integrator::Xml
            {
                // std::unique_ptr<core::Filter::Xml> filter;
                int sample_count;
                float cutoff_probability;
                // float rr_threshold;

                explicit Xml(const xml::Node &node);
                std::unique_ptr<Integrator> create() const override;
            };

        public:
            explicit Pathtracer(const Pathtracer::Xml &xml) : Pathtracer(xml.sample_count, xml.cutoff_probability){};
            explicit Pathtracer(int spp = 16, float cutoff_probability = 0.2);

            void integrate(const core::Scene &scene, core::Image &output) override;

        private:
            int m_spp;
            float m_cutoff_probability;
            std::mutex m_mtx;
            int m_progress;

        private:
            const glm::vec3 estimatePixel(const core::Scene &scene, const geometry::Ray &ray, std::shared_ptr<core::UniformSampler> sampler, int depth, bool light_explicitly_sampled = false);
            void integratePatch(const core::Scene &scene, core::Image &output, int row_begin, int row_end, int id);
        };
    }
}

#endif
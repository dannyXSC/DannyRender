#ifndef __DANNY__INTEGRATOR__PATHTRACER__
#define __DANNY__INTEGRATOR__PATHTRACER__

#include "core/Scene.h"
#include "core/Image.h"

#include <integrator/Integrator.h>
#include <geometry/Ray.hpp>

namespace danny
{
    namespace integrator
    {
        class Pathtracer : public Integrator
        {
        public:
            explicit Pathtracer(int spp = 16, float cutoff_probability = 0.2, int thread_num = 6);

            void integrate(const core::Scene &scene, core::Image &output) override;

        private:
            int m_spp;
            float m_cutoff_probability;
            int m_thread_num;

        private:
            const glm::vec3 estimatePixel(const core::Scene &scene, const geometry::Ray &ray, std::shared_ptr<core::UniformSampler> sampler, bool light_explicitly_sampled = false);
            //            void integratePatch(const core::Scene &scene, core::Image &output, int row_begin, int row_end);
        };
    }
}

#endif
#ifndef __DANNY__CORE__DISCRETE1DSAMPLER__
#define __DANNY__CORE__DISCRETE1DSAMPLER__

#include "Sampler.hpp"

namespace danny
{
    namespace core
    {
        class Discrete1DSampler
        {
        public:
            Discrete1DSampler() = default;
            explicit Discrete1DSampler(const std::vector<float> &pdf);

            int sample(std::shared_ptr<core::UniformSampler> sampler) const;
            float getPdf(int x) const;

            float get_sum() const { return m_sum; }

        private:
            std::vector<float> m_cdf;
            float m_sum;
        };
    }
}

#endif
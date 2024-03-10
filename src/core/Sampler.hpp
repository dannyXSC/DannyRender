#ifndef __DANNY__CORE__SAMPLER__
#define __DANNY__CORE__SAMPLER__
#include <core/forward_decl.h>

#include <random>
#include <map>
#include <utility>
#include <memory>
namespace danny
{
    namespace core
    {
        class RealSampler
        {
        public:
            virtual ~RealSampler() = default;

            virtual float sample() = 0;
        };

        class UniformSampler : public RealSampler
        {
        public:
            UniformSampler(float min = 0.0f, float max = 1.0f);

            float sample() override;

        private:
            std::mt19937 m_generator;
            std::uniform_real_distribution<float> m_distribution;
        };
    }
}
#endif